/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef MEMETIC_MDD_SOLVER_HPP_
#define MEMETIC_MDD_SOLVER_HPP_

#include <set>
#include "MDDSolver.hpp"
#include "MDDChart.hpp"
#include "MDDSolution.hpp"
#include "LocalSearchMDDSolver.hpp"

class MemeticMDDSolver : public MDDSolver {
    std::mt19937 rgen;
    unsigned seed;
    unsigned chromosome_count;
    float crossover_probability;
    float mutation_probability;
    unsigned max_num_eval;
    float percentaje_to_select;
    bool sort;

public:
    MemeticMDDSolver() = delete;
    MemeticMDDSolver(const MemeticMDDSolver& g) = delete;
    MemeticMDDSolver(MemeticMDDSolver&& g) = delete;
    MemeticMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned chromosome_count, float crossover_probability, float mutation_probability, unsigned max_num_eval, float percentaje_to_select, bool sort);

    std::vector<MDDSolution> select_ls(std::vector<MDDSolution>& population, float percentaje_to_select, bool order);

    [[nodiscard]] MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* MEMETIC_MDD_SOLVER_HPP_ */