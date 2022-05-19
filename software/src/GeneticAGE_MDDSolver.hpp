/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef GENETIC_AGE_MDD_SOLVER_HPP_
#define GENETIC_AGE_MDD_SOLVER_HPP_

#include "MDDSolver.hpp"
#include "MDDChart.hpp"
#include "MDDSolution.hpp"
#include "GeneticMDDSolver.hpp"

class GeneticAGE_MDDSolver : public GeneticMDDSolver {
    static constexpr unsigned num_selected_per_generation = 2;
    static constexpr float crossover_probability = 1.0;

protected:
    crossover_func_t crossover_method;
    unsigned chromosome_count;
    float mutation_probability;

    void reemplace(population_t& to_be_reemplaced, population_t& selected) noexcept;

public:
    GeneticAGE_MDDSolver() = delete;
    GeneticAGE_MDDSolver(const GeneticAGE_MDDSolver& g) = delete;
    GeneticAGE_MDDSolver(GeneticAGE_MDDSolver&& g) = delete;
    GeneticAGE_MDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned chromosome_count, float mutation_probability);

    inline void set_crossover_method(crossover_func_t crossover_method) noexcept { this->crossover_method = crossover_method; }

    [[nodiscard]] MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* GENETIC_AGE_MDD_SOLVER_HPP_ */