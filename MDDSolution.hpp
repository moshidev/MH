#ifndef MDD_SOLUTION_HPP_
#define MDD_SOLUTION_HPP_

#include <map>
#include "MDDChart.hpp"

class MDDSolution {
public:
    typedef unsigned index_t;
    typedef unsigned long sum_to_other_indexes_in_solution_t;
    typedef std::map<index_t,sum_to_other_indexes_in_solution_t> solution_t;
    solution_t solution;

public:
    sum_to_other_indexes_in_solution_t calc_distance_summatory_from_vertex_to_solution(index_t v, const MDDChart& chart) const noexcept;
    void update(index_t index, sum_to_other_indexes_in_solution_t sum) noexcept;
    unsigned calc_dispersion(void) const noexcept;
    inline const solution_t& get_solution(void) const noexcept { return solution; }
};

#endif /* MDD_SOLUTION_HPP_ */