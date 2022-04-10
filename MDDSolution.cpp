#include "MDDSolution.hpp"

typedef MDDSolution::sum_to_other_indexes_in_solution_t sum_t;

MDDSolution::MDDSolution(const std::shared_ptr<const MDDChart>& c)
:chart{c}
{   }

sum_t MDDSolution::distance_summatory_from_index_to_solution(index_t v) const noexcept {
    sum_t sum = 0;
    for (const auto& u : solution) {
        sum += chart->at(v, u.first);
    }
    return sum;
}

void MDDSolution::update(index_t index, sum_t sum) noexcept {
    if (solution.find(index) == solution.end()) {
        solution.insert(std::pair<index_t, sum_t>(index, 0));
    }
    solution.at(index) += sum;
}

unsigned MDDSolution::calc_dispersion(void) const noexcept {
    sum_t min = std::numeric_limits<unsigned>::max();
    sum_t max = std::numeric_limits<unsigned>::min();

    for (const auto& a : solution) {
        sum_t sum = a.second;
        max = std::max(max, sum);
        min = std::min(min, sum);
    }

    return max-min;
}