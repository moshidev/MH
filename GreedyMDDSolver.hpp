#ifndef GREEDY_MDD_SOLVER_HPP_
#define GREEDY_MDD_SOLVER_HPP_

#include <set>
#include "MDDSolver.hpp"
#include "MDDChart.hpp"
#include "MDDSolution.hpp"

class GreedyMDDSolver : public MDDSolver {
    MDDSolution::index_t select_next_element(const MDDSolution& solution, const std::set<MDDSolution::index_t>& nonchosen) const noexcept;
    void init_nonchosen(std::set<MDDSolution::index_t>& nonchosen) noexcept;

public:
    GreedyMDDSolver() = delete;
    GreedyMDDSolver(const GreedyMDDSolver& g) = delete;
    GreedyMDDSolver(GreedyMDDSolver&& g) = delete;
    GreedyMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c);

    [[nodiscard]] MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* GREEDY_MDD_SOLVER_HPP_ */