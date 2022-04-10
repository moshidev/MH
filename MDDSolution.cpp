#include "MDDSolution.hpp"

unsigned long MDDSolution::calc_distance_summatory_from_vertex_to_solution(unsigned v, const MDDChart& chart) const noexcept {
    unsigned long sum = 0;
    for (const auto& u : solution) {
        sum += chart.at(v, u.first);
    }
    return sum;
}

void MDDSolution::update(unsigned index, unsigned long sum) noexcept {
    if (solution.find(index) == solution.end()) {
        solution.insert(std::pair<unsigned, unsigned long>(index, 0));
    }
    solution.at(index) += sum;
}

unsigned MDDSolution::calc_dispersion(void) const noexcept {
    unsigned min = std::numeric_limits<unsigned>::max();
    unsigned max = std::numeric_limits<unsigned>::min();

    for (const auto& a : solution) {
        unsigned sum = a.second;
        max = std::max(max, sum);
        min = std::min(min, sum);
    }

    return max-min;
}