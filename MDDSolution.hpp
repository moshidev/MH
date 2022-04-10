#ifndef MDD_SOLUTION_HPP_
#define MDD_SOLUTION_HPP_

#include <map>
#include "MDDChart.hpp"

class MDDSolution {
public:
    typedef unsigned index_t;
    typedef unsigned long sum_to_other_indexes_in_solution_t;
    std::map<index_t,sum_to_other_indexes_in_solution_t> solution;

public:
    unsigned long calc_distance_summatory_from_vertex_to_solution(unsigned v, const MDDChart& chart) const noexcept;
    void update(unsigned index, unsigned long sum) noexcept;
    unsigned calc_dispersion(void) const noexcept;
    inline const std::map<unsigned,unsigned long>& get_solution(void) const noexcept { return solution; }
};

#endif /* MDD_SOLUTION_HPP_ */