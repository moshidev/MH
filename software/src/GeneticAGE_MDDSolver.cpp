#include "GeneticAGE_MDDSolver.hpp"

GeneticAGE_MDDSolver::GeneticAGE_MDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned chromosome_count, float mutation_probability)
:GeneticMDDSolver{seed, c}, chromosome_count{chromosome_count}, mutation_probability{mutation_probability}
{   }

void GeneticAGE_MDDSolver::reemplace(population_t& to_be_reemplaced, population_t& selected) noexcept {
    for (int i = 0; i < selected.size(); i++) {
        unsigned worst_solution_index = find_worst_solution_pos_in_population(to_be_reemplaced);
        MDDSolution worst_solution = to_be_reemplaced[worst_solution_index];
        if (selected[i].calc_dispersion() < worst_solution.calc_dispersion()) {
            to_be_reemplaced.erase(to_be_reemplaced.begin()+worst_solution_index);
            to_be_reemplaced.push_back(selected[i]);
        }
    }
}

MDDSolution GeneticAGE_MDDSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
    MDDSolution solution{chart};

    population = generate_random_population(chromosome_count, number_of_elements_to_be_chosen);
    solution = population[find_best_solution_pos_in_population(population)];

    for (int i = 0; i < 10'000'000; i++) {
        population_t selected = select_by_binary_tournament(population, num_selected_per_generation);
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