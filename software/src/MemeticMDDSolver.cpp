/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include "MemeticMDDSolver.hpp"
#include "GeneticMDD.hpp"
#include "LocalSearchMDDSolver.hpp"

MemeticMDDSolver::MemeticMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned chromosome_count, float crossover_probability, float mutation_probability, unsigned max_num_eval, float percentaje_to_select, bool sort)
:MDDSolver{seed, c}, rgen{seed}, seed{seed}, chromosome_count{chromosome_count}, crossover_probability{crossover_probability}, mutation_probability{mutation_probability}, max_num_eval{max_num_eval}, percentaje_to_select{percentaje_to_select}, sort{sort}
{	}

std::vector<MDDSolution> MemeticMDDSolver::select_ls(std::vector<MDDSolution>& population, float percentaje_to_select, bool sort) {
	std::vector<MDDSolution> retv;

	if (sort) {
		std::sort(population.begin(), population.end());
	}
	else {
		std::shuffle(population.begin(), population.end(), rgen);
	}

	retv.insert(retv.begin(), population.begin(), population.begin()+percentaje_to_select*population.size());
	population.erase(population.begin(), population.begin()+percentaje_to_select*population.size());

	return retv;
}

MDDSolution MemeticMDDSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
	LocalSearchMDDSolver ls{seed, chart};
	GeneticMDD genetic{seed, chromosome_count, crossover_probability, mutation_probability, GeneticMDD::crossover_uniform, GeneticMDD::reemplace_generational};

	GeneticMDD::population_t population = genetic.generate_random_population(chart, chromosome_count, number_of_elements_to_be_chosen);
	unsigned n_eval = 0;
	while (n_eval < max_num_eval) {
		for (int i = 0; i < 10; i++) {
			n_eval += genetic.generation_step(population);
		}
		auto selected = select_ls(population, percentaje_to_select, sort);
		for (auto& s : selected) {
			auto res = ls.local_search(s, number_of_elements_to_be_chosen, 400);
			n_eval += res.second;
			population.push_back(res.first);
		}
	}

	return *find_best_solution_in_population(population);
}