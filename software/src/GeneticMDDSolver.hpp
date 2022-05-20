/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef GENETIC_MDD_SOLVER_HPP_
#define GENETIC_MDD_SOLVER_HPP_

#include <vector>
#include <functional>
#include "MDDSolver.hpp"
#include "MDDChart.hpp"
#include "MDDSolution.hpp"

class GeneticMDDSolver : public MDDSolver {
protected:
    typedef std::vector<MDDSolution> population_t;
    typedef std::function<std::pair<MDDSolution,MDDSolution>(GeneticMDDSolver*,const MDDSolution&,const MDDSolution&)> crossover_func_t;
    population_t population;
    unsigned num_eval;

    population_t generate_random_population(int num_chromosomes, int num_genes) noexcept;
    population_t select_by_binary_tournament(const population_t& p, int num_to_be_selected) noexcept;
    void crossover(population_t& p, float probability, crossover_func_t f) noexcept;
    void mutate(population_t& p, float probability) noexcept;
    void repair(MDDSolution& sol) noexcept;

    std::vector<bool> get_binary_representation_from(const MDDSolution& sol) const noexcept;

    unsigned find_best_solution_pos_in_population(const population_t& p) noexcept;
    unsigned find_worst_solution_pos_in_population(const population_t& p) noexcept;

public:
    GeneticMDDSolver() = delete;
    GeneticMDDSolver(const GeneticMDDSolver& g) = delete;
    GeneticMDDSolver(GeneticMDDSolver&& g) = delete;
    GeneticMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c);

    static std::pair<MDDSolution,MDDSolution> crossover_uniform(GeneticMDDSolver* self, const MDDSolution& a, const MDDSolution& b);
    static std::pair<MDDSolution,MDDSolution> crossover_position(GeneticMDDSolver* self, const MDDSolution& a, const MDDSolution& b);
    
    [[nodiscard]] virtual MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept = 0;
};

#endif /* GENETIC_MDD_SOLVER_HPP_ */