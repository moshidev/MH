/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef BMB_SOLVER_HPP_
#define BMB_SOLVER_HPP_

#include "MDDSolver.hpp"
#include "MDDChart.hpp"
#include "MDDSolution.hpp"
#include "LocalSearchMDDSolver.hpp"

class BMBSolver : public MDDSolver {
	LocalSearchMDDSolver ls_solver;
	unsigned num_searches;
	unsigned evals_per_search;

	MDDSolution generate_random_solution(unsigned number_of_elements_to_be_chosen);

public:
	BMBSolver() = delete;
    BMBSolver(const BMBSolver& g) = delete;
    BMBSolver(BMBSolver&& g) = delete;
	BMBSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned num_searches, unsigned evals_per_search);

    [[nodiscard]] MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* BMB_SOLVER_HPP_ */