/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef MDD_SOLUTION_HPP_
#define MDD_SOLUTION_HPP_

#include <map>
#include <vector>
#include <memory>
#include "MDDChart.hpp"

class MDDSolution {
public:
    typedef unsigned index_t;
    typedef unsigned long sum_to_other_indexes_in_solution_t;
    typedef std::map<index_t,sum_to_other_indexes_in_solution_t> solution_t;

private:
    solution_t solution;
    sum_to_other_indexes_in_solution_t maximum;
    sum_to_other_indexes_in_solution_t minimum;
    std::shared_ptr<const MDDChart> chart;

    void update_maximum_and_minimum(void);

public:
    MDDSolution() = delete;
    MDDSolution(const MDDSolution& sol) = default;
    MDDSolution(MDDSolution&& sol) = default;
    MDDSolution(const std::shared_ptr<const MDDChart>& c);
    inline MDDSolution& operator=(const MDDSolution& sol) = default;
    inline MDDSolution& operator=(MDDSolution&& sol) = default;

    sum_to_other_indexes_in_solution_t distance_summatory_from_index_to_solution(index_t v) const noexcept;
    unsigned calc_dispersion(void) const noexcept;
    unsigned calc_index_candidate_dispersion(index_t index) const noexcept;

    bool add_index_to_solution(index_t index) noexcept;
    bool remove_index_from_solution(index_t index) noexcept;

    inline bool is_valid(void) const noexcept { return solution.size() == chart->num_elements_to_be_chosen(); }
    inline const MDDChart& get_chart(void) const noexcept { return *chart; }
    inline const std::shared_ptr<const MDDChart>& get_chart_pointer(void) const noexcept { return chart; }
    inline const solution_t& get_solution(void) const noexcept { return solution; }
    std::vector<index_t> get_solution_vector(void) const noexcept;

    MDDSolution intersect(const MDDSolution& rh) const noexcept;

    inline bool operator<(const MDDSolution& rh) const noexcept { return calc_dispersion() < rh.calc_dispersion(); }
    inline bool operator>(const MDDSolution& rh) const noexcept { return calc_dispersion() > rh.calc_dispersion(); }
    inline bool operator==(const MDDSolution& rh) const noexcept  { return calc_dispersion() == rh.calc_dispersion(); }
};

#endif /* MDD_SOLUTION_HPP_ */