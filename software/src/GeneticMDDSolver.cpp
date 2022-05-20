/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include <random>
#include "GeneticMDDSolver.hpp"

GeneticMDDSolver::GeneticMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, const GeneticMDD& genetic_alg, unsigned max_num_eval, unsigned num_chromosomes)
:MDDSolver{seed, c}, genetic_alg{genetic_alg}, max_num_eval{max_num_eval}, num_chromosomes{num_chromosomes}
{   }

MDDSolution GeneticMDDSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
    MDDSolution solution{chart};
    unsigned num_eval = 0;

    GeneticMDD::population_t population = genetic_alg.generate_random_population(chart, num_chromosomes, number_of_elements_to_be_chosen);
    solution = *find_best_solution_in_population(population);
    
    while (num_eval < max_num_eval) {
        num_eval += genetic_alg.generation_step(population);
        solution = *find_best_solution_in_population(population);
    }

    return solution;
}