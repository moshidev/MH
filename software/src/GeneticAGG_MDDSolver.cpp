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

    population = generate_random_population(chromosome_count, number_of_elements_to_be_chosen);
    solution = population[find_best_solution_pos_in_population(population)];

    for (int i = 0; i < 1000; i++) {
        population_t selected = select_by_binary_tournament(population, chromosome_count);
        crossover(selected, crossover_probability, crossover_method);
        mutate(selected, mutation_probability);
        reemplace(population, selected);

        solution = population[find_best_solution_pos_in_population(population)];
    }

    return solution;
}