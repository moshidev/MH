/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef LOCAL_SEARCH_MDD_SOLVER_HPP_
#define LOCAL_SEARCH_MDD_SOLVER_HPP_

#include "MDDSolution.hpp"
#include "MDDSolver.hpp"

class LocalSearchMDDSolver : public MDDSolver {
    std::pair<MDDChart::index_t,unsigned> best_first_neighbour_from_index(MDDChart::index_t index, const MDDSolution& solution, const std::set<MDDChart::index_t>& nonchosen) const noexcept;

public:
    LocalSearchMDDSolver() = delete;
    LocalSearchMDDSolver(const LocalSearchMDDSolver& g) = delete;
    LocalSearchMDDSolver(LocalSearchMDDSolver&& g) = delete;
    LocalSearchMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& chart);

    std::pair<MDDSolution,unsigned> local_search(const MDDSolution& ini, unsigned number_of_elements_to_be_chosen, unsigned max_num_of_neighbour_eval);

    [[nodiscard]] MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* LOCAL_SEARCH_MDD_SOLVER_HPP_ */