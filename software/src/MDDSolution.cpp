/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include <limits>
#include "MDDSolution.hpp"

typedef MDDSolution::sum_to_other_indexes_in_solution_t sum_t;

MDDSolution::MDDSolution(const std::shared_ptr<const MDDChart>& c)
:maximum{0}, minimum{0}, chart{c}
{   }

sum_t MDDSolution::distance_summatory_from_index_to_solution(index_t v) const noexcept {
    sum_t sum = 0;
    for (const auto& u : solution) {
        sum += chart->at(v, u.first);
    }
    return sum;
}

void MDDSolution::update_maximum_and_minimum(void) {
    [[unlikely]] if (solution.empty()) {
        maximum = 0;
        minimum = 0;
        return;
    }

    sum_t max = std::numeric_limits<sum_t>::min();
    sum_t min = std::numeric_limits<sum_t>::max();
    for (const auto& e : solution) {
        max = std::max(max, e.second);
        min = std::min(min, e.second);
    }
    maximum = max;
    minimum = min;
}

void MDDSolution::add_index_to_solution(index_t index) noexcept {
    [[unlikely]] if (solution.find(index) != solution.end()) {
        return;
    }

    for (auto& e : solution) {
        e.second += chart->at(index, e.first);
    }
    solution.emplace(index, distance_summatory_from_index_to_solution(index));
    update_maximum_and_minimum();
}

void MDDSolution::remove_index_from_solution(index_t index) noexcept {
    [[unlikely]] if (solution.find(index) == solution.end()) {
        return;
    }

    for (auto& e : solution) {
        e.second -= chart->at(index, e.first);
    }
    solution.erase(index);
    update_maximum_and_minimum();
}

unsigned MDDSolution::calc_dispersion(void) const noexcept {
    return maximum-minimum;
}

unsigned MDDSolution::calc_index_candidate_dispersion(index_t index) const noexcept {
    sum_t min = std::numeric_limits<sum_t>::max();
    sum_t max = std::numeric_limits<sum_t>::min();
    sum_t distance_summatory_vs = distance_summatory_from_index_to_solution(index);
    max = std::max(max, distance_summatory_vs);
    min = std::min(min, distance_summatory_vs);

    for (const auto& a : solution) {
        sum_t sum = a.second + chart->at(index, a.first);
        max = std::max(max, sum);
        min = std::min(min, sum);
    }

    return max-min;
}

MDDSolution MDDSolution::intersect(const MDDSolution& rh) const noexcept {
    MDDSolution res{chart};

    auto lit = solution.cbegin();
    auto rit = rh.solution.cbegin();

    while (lit != solution.end() && rit != rh.solution.end()) {
        if (lit->first == rit->first) {
            res.add_index_to_solution(lit->first);
            ++lit;
            ++rit;
        }
        else if (lit->first < rit->first) {
            ++lit;
        }
        else {
            ++rit;
        }
    }

    return res;
}