#ifndef MDD_SOLUTION_HPP_
#define MDD_SOLUTION_HPP_

#include <map>

class MDDSolution {
    std::map<unsigned,unsigned long> index_maps_distance_summatory;
public:
    inline const std::map<unsigned,unsigned long>& get_solution(void) const noexcept { return index_maps_distance_summatory; }
    void update(unsigned index, unsigned long sum) noexcept;
    unsigned calc_dispersion(void) const noexcept;
};

#endif /* MDD_SOLUTION_HPP_ */