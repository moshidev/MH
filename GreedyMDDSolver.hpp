#ifndef GREEDY_MDD_SOLVER_HPP_
#define GREEDY_MDD_SOLVER_HPP_

#include <random>
#include <set>
#include "MDDChart.hpp"

class GreedyMDDSolver {
    MDDChart chart;
    std::mt19937 rgenerator;
    std::uniform_int_distribution<std::uint_fast32_t> udistribution;

    unsigned long calc_distance_summatory_from_vertex_to_solution(unsigned v, const std::set<unsigned>& solution) const noexcept;
    unsigned select_random_element(void) noexcept;
    unsigned select_next_element(const std::set<unsigned>& solution, const std::set<unsigned>& nonchosen) const noexcept;
    void init_nonchosen(std::set<unsigned>& nonchosen) noexcept;

public:
    GreedyMDDSolver() = delete;
    GreedyMDDSolver(const GreedyMDDSolver& g) = delete;
    GreedyMDDSolver(GreedyMDDSolver&& g) = delete;
    GreedyMDDSolver(unsigned seed, const MDDChart& c);
    GreedyMDDSolver(unsigned seed, MDDChart&& c);

    inline const MDDChart& get_chart(void) const noexcept { return chart; }
    unsigned long calc_solution_dispersion(const std::set<unsigned>& solution) const noexcept;
    [[nodiscard]] std::set<unsigned> solve(unsigned number_of_elements_to_be_chosen) noexcept;
};

#endif /* GREEDY_MDD_SOLVER_HPP_ */