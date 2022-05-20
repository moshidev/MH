#include "GeneticAGG_MDDSolver.hpp"

GeneticAGG_MDDSolver::GeneticAGG_MDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned chromosome_count, float crossover_probability, float mutation_probability)
:GeneticMDDSolver{seed, c}, chromosome_count{chromosome_count}, crossover_probability{crossover_probability}, mutation_probability{mutation_probability}
{   }

void GeneticAGG_MDDSolver::reemplace(population_t& to_be_reemplaced, population_t& selected) noexcept {
    MDDSolution best_solution{to_be_reemplaced[find_best_solution_pos_in_population(to_be_reemplaced)]};
    to_be_reemplaced.swap(selected);
    to_be_reemplaced.erase(to_be_reemplaced.begin()+find_worst_solution_pos_in_population(to_be_reemplaced));
    to_be_reemplaced.push_back(best_solution);
}

MDDSolution GeneticAGG_MDDSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
    MDDSolution solution{chart};
    num_eval = 0;

    population = generate_random_population(chromosome_count, number_of_elements_to_be_chosen);
    solution = population[find_best_solution_pos_in_population(population)];

    while (num_eval < 100'000) {
        population_t selected = select_by_binary_tournament(population, chromosome_count);
        num_eval += selected.size();
        crossover(selected, crossover_probability, crossover_method);
        mutate(selected, mutation_probability);
        reemplace(population, selected);

        MDDSolution best_in_population = population[find_best_solution_pos_in_population(population)];
        if (best_in_population.calc_dispersion() < solution.calc_dispersion()) {
            solution = best_in_population;
        }
    }

    return solution;
}