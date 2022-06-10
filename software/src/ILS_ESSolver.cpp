#include "ILS_ESSolver.hpp"

ILS_ESSolver::ILS_ESSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned num_searches, unsigned evals_per_search, float mutation_probability, unsigned max_num_neighbours, unsigned max_num_successes, float final_temp)
:MDDSolver{seed, c}, es_solver{seed, c, evals_per_search, max_num_neighbours, max_num_successes, final_temp}, num_searches{num_searches}, mutation_probability{mutation_probability}
{	}

void ILS_ESSolver::mutate(MDDSolution& sol, float probability) noexcept {
	unsigned num_mutations = sol.get_solution().size() * probability;
	std::set<MDDSolution::index_t> chosen;
	std::set<MDDSolution::index_t> nonchosen;
	init_nonchosen(nonchosen, sol);
	for (auto& a : sol.get_solution()) {
		chosen.insert(a.first);
	}

	for (int i = 0; i < num_mutations; i++) {
		unsigned ci = choose_random(chosen);
		unsigned cj = choose_random(nonchosen);
		sol.remove_index_from_solution(ci);
		sol.add_index_to_solution(cj);
	}
}

MDDSolution ILS_ESSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
	MDDSolution best_solution{generate_random_solution(number_of_elements_to_be_chosen)};

	if (num_searches > 0) {
		best_solution = es_solver.simulated_annealing(best_solution, number_of_elements_to_be_chosen).first;
	}
	for (int i = 0; i < num_searches-1; i++) {
		MDDSolution mutated{best_solution};
		mutate(mutated, mutation_probability);
		MDDSolution es_solution{es_solver.simulated_annealing(mutated, number_of_elements_to_be_chosen).first};
		if (es_solution.calc_dispersion() < best_solution.calc_dispersion()) {
			best_solution = es_solution;
		}
	}

	return best_solution;
}