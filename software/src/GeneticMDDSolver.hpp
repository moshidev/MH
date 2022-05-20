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
#include "GeneticMDD.hpp"

class GeneticMDDSolver : public MDDSolver {
protected:
    GeneticMDD genetic_alg;
    unsigned max_num_eval;
    unsigned num_chromosomes;

public:
    GeneticMDDSolver() = delete;
    GeneticMDDSolver(const GeneticMDDSolver& g) = delete;
    GeneticMDDSolver(GeneticMDDSolver&& g) = delete;
    GeneticMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, const GeneticMDD& genetic_alg, unsigned max_num_eval, unsigned num_chromosomes);

    [[nodiscard]] virtual MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* GENETIC_MDD_SOLVER_HPP_ */