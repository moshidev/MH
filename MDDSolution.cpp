#include "MDDSolution.hpp"

unsigned long MDDSolution::calc_distance_summatory_from_vertex_to_solution(unsigned v, const MDDChart& chart) const noexcept {
    unsigned long sum = 0;
    for (const auto& u : index_maps_distance_summatory) {
        sum += chart.at(v, u.first);
    }
    return sum;
}

void MDDSolution::update(unsigned index, unsigned long sum) noexcept {
    if (index_maps_distance_summatory.find(index) == index_maps_distance_summatory.end()) {
        index_maps_distance_summatory.insert(std::pair<unsigned, unsigned long>(index, 0));
    }
    index_maps_distance_summatory.at(index) += sum;
}

unsigned MDDSolution::calc_dispersion(void) const noexcept {
    unsigned min = std::numeric_limits<unsigned>::max();
    unsigned max = std::numeric_limits<unsigned>::min();

    for (const auto& a : index_maps_distance_summatory) {
        unsigned sum = a.second;
        max = std::max(max, sum);
        min = std::min(min, sum);
    }

    return max-min;
}