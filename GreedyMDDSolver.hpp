#ifndef GREEDY_MDD_SOLVER_HPP_
#define GREEDY_MDD_SOLVER_HPP_

#include <set>
#include "MDDSolver.hpp"
#include "MDDChart.hpp"
#include "MDDSolution.hpp"

class GreedyMDDSolver : public MDDSolver {

    unsigned calc_distance_summatory_from_vertex_to_solution(unsigned v, const MDDSolution& solution) const noexcept;
    unsigned calc_vertex_candidate_dispersion(unsigned v, const MDDSolution& solution) const noexcept;
    unsigned select_next_element(const MDDSolution& solution, const std::set<unsigned>& nonchosen) const noexcept;
    void init_nonchosen(std::set<unsigned>& nonchosen) noexcept;

public:
    GreedyMDDSolver() = delete;
    GreedyMDDSolver(const GreedyMDDSolver& g) = delete;
    GreedyMDDSolver(GreedyMDDSolver&& g) = delete;
    GreedyMDDSolver(unsigned seed, const MDDChart& c);
    GreedyMDDSolver(unsigned seed, MDDChart&& c);

    [[nodiscard]] MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* GREEDY_MDD_SOLVER_HPP_ */