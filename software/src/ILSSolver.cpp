/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include "ILSSolver.hpp"
#include <set>

ILSSolver::ILSSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned num_searches, unsigned evals_per_search, float mutation_probability)
:MDDSolver{seed, c},ls_solver{seed, c}, num_searches{num_searches}, evals_per_search{evals_per_search}, mutation_probability{mutation_probability}
{	}

void ILSSolver::mutate(MDDSolution& sol, float probability) noexcept {
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

MDDSolution ILSSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
	MDDSolution best_solution{generate_random_solution(number_of_elements_to_be_chosen)};

	if (num_searches > 0) {
		best_solution = ls_solver.local_search(best_solution, number_of_elements_to_be_chosen, evals_per_search).first;
	}
	for (int i = 0; i < num_searches-1; i++) {
		MDDSolution mutated{best_solution};
		mutate(mutated, mutation_probability);
		MDDSolution ls_solution{ls_solver.local_search(mutated, number_of_elements_to_be_chosen, evals_per_search).first};
		if (ls_solution < best_solution) {
			best_solution = ls_solution;
		}
	}

	return best_solution;
}