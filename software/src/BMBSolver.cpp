/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include "BMBSolver.hpp"
#include <set>

BMBSolver::BMBSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned num_searches, unsigned evals_per_search)
:MDDSolver{seed, c}, ls_solver{seed, c}, num_searches{num_searches}, evals_per_search{evals_per_search}
{	}

MDDSolution BMBSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
	std::set<MDDSolution> solutions;

	for (int i = 0; i < num_searches; i++) {
		MDDSolution random_sol{generate_random_solution(number_of_elements_to_be_chosen)};
		solutions.insert(ls_solver.local_search(random_sol, number_of_elements_to_be_chosen, evals_per_search).first);
	}

	return *solutions.begin();	
}