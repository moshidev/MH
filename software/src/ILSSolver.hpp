/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef ILS_SOLVER_HPP_
#define ILS_SOLVER_HPP_

#include "MDDSolver.hpp"
#include "MDDChart.hpp"
#include "MDDSolution.hpp"
#include "LocalSearchMDDSolver.hpp"

class ILSSolver : public MDDSolver {
    LocalSearchMDDSolver ls_solver;
    unsigned num_searches;
    unsigned evals_per_search;
    float mutation_probability;

    void mutate(MDDSolution& sol, float probability) noexcept;

public:
	ILSSolver() = delete;
    ILSSolver(const ILSSolver& g) = delete;
    ILSSolver(ILSSolver&& g) = delete;
	ILSSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned num_searches, unsigned evals_per_search, float mutation_probability);

    [[nodiscard]] MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* ILS_SOLVER_HPP_ */