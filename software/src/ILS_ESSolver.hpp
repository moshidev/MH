/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef ILS_ES_SOLVER_HPP_
#define ILS_ES_SOLVER_HPP_

#include "MDDSolver.hpp"
#include "MDDChart.hpp"
#include "MDDSolution.hpp"
#include "SimulatedAnnealingMDDSolver.hpp"

class ILS_ESSolver : public MDDSolver {
    SimulatedAnnealingMDDSolver es_solver;
	unsigned num_searches;
    float mutation_probability;

    void mutate(MDDSolution& sol, float probability) noexcept;

public:
	ILS_ESSolver() = delete;
    ILS_ESSolver(const ILS_ESSolver& g) = delete;
    ILS_ESSolver(ILS_ESSolver&& g) = delete;
	ILS_ESSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned num_searches, unsigned evals_per_search, float mutation_probability, unsigned max_num_neighbours, unsigned max_num_successes, float final_temp);

    [[nodiscard]] MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* ILS_ES_SOLVER_HPP_ */