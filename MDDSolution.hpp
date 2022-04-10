#ifndef MDD_SOLUTION_HPP_
#define MDD_SOLUTION_HPP_

#include <map>
#include "MDDChart.hpp"

class MDDSolution {
    std::map<unsigned,unsigned long> index_maps_distance_summatory;
public:
    inline const std::map<unsigned,unsigned long>& get_solution(void) const noexcept { return index_maps_distance_summatory; }

    unsigned long calc_distance_summatory_from_vertex_to_solution(unsigned v, const MDDChart& chart) const noexcept;

    void update(unsigned index, unsigned long sum) noexcept;
    unsigned calc_dispersion(void) const noexcept;
};

#endif /* MDD_SOLUTION_HPP_ */