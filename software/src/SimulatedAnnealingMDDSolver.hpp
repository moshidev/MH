/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef SIMULATED_ANNEALING_MDD_SOLVER_HPP_
#define SIMULATED_ANNEALING_MDD_SOLVER_HPP_

#include <set>
#include "MDDSolver.hpp"
#include "MDDChart.hpp"
#include "MDDSolution.hpp"

class SimulatedAnnealingMDDSolver : public MDDSolver {
protected:
    constexpr static float boltzmann_constant = 1.380649E-23;
    std::mt19937 rgen;
	unsigned max_num_eval;
	unsigned max_num_neighbours;
	unsigned max_num_successes;
	float final_temp;

    float cauchy_annealing_policy(float initial_temperature, float last_temperature, unsigned total_num_coolings_we_will_be_doing) const noexcept;
    float calc_initial_temp(float micro, float initial_sol_cost, float fi) const noexcept;
    MDDSolution make_random_neighbour_from_solution(const MDDSolution& s) noexcept;
    bool probabilistic_acceptance(float delta_f, float temperature) noexcept;

public:
    SimulatedAnnealingMDDSolver() = delete;
    SimulatedAnnealingMDDSolver(const SimulatedAnnealingMDDSolver& g) = delete;
    SimulatedAnnealingMDDSolver(SimulatedAnnealingMDDSolver&& g) = delete;
    SimulatedAnnealingMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned max_num_eval, unsigned max_num_neighbours, unsigned max_num_successes, float final_temp);

    std::pair<MDDSolution,unsigned> simulated_annealing(const MDDSolution& ini, unsigned number_of_elements_to_be_chosen) noexcept;

    [[nodiscard]] MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* SIMULATED_ANNEALING_MDD_SOLVER_HPP_ */