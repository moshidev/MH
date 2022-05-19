/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef GENETIC_AGG_MDD_SOLVER_HPP_
#define GENETIC_AGG_MDD_SOLVER_HPP_

#include "MDDSolver.hpp"
#include "MDDChart.hpp"
#include "MDDSolution.hpp"
#include "GeneticMDDSolver.hpp"

class GeneticAGG_MDDSolver : public GeneticMDDSolver {
protected:
    crossover_func_t crossover_method;
    unsigned chromosome_count;
    float crossover_probability;
    float mutation_probability;

    void reemplace(population_t& to_be_reemplaced, population_t& selected) noexcept;

public:
    GeneticAGG_MDDSolver() = delete;
    GeneticAGG_MDDSolver(const GeneticAGG_MDDSolver& g) = delete;
    GeneticAGG_MDDSolver(GeneticAGG_MDDSolver&& g) = delete;
    GeneticAGG_MDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned chromosome_count, float crossover_probability, float mutation_probability);

    inline void set_crossover_method(crossover_func_t crossover_method) noexcept { this->crossover_method = crossover_method; }

    [[nodiscard]] MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* GENETIC_AGG_MDD_SOLVER_HPP_ */