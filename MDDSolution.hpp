#ifndef MDD_SOLUTION_HPP_
#define MDD_SOLUTION_HPP_

#include <map>
#include <memory>
#include "MDDChart.hpp"

class MDDSolution {
public:
    typedef unsigned index_t;
    typedef unsigned long sum_to_other_indexes_in_solution_t;
    typedef std::map<index_t,sum_to_other_indexes_in_solution_t> solution_t;
    solution_t solution;
    const std::shared_ptr<const MDDChart> chart;

public:
    MDDSolution() = delete;
    MDDSolution(const std::shared_ptr<const MDDChart>& c);

    sum_to_other_indexes_in_solution_t distance_summatory_from_index_to_solution(index_t v) const noexcept;
    unsigned calc_dispersion(void) const noexcept;

    void update(index_t index, sum_to_other_indexes_in_solution_t sum) noexcept;

    inline bool is_valid(bool) const noexcept { return solution.size() == chart->num_elements_to_be_chosen(); }
    inline const solution_t& get_solution(void) const noexcept { return solution; }
    
};

#endif /* MDD_SOLUTION_HPP_ */