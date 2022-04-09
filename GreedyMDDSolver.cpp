#include <limits>
#include <vector>
#include <set>
#include "GreedyMDDSolver.hpp"

GreedyMDDSolver::GreedyMDDSolver(unsigned seed, const MDDChart& c)
:chart{c}, rgenerator{seed}, udistribution{0, c.num_elements()-1}
{   }

GreedyMDDSolver::GreedyMDDSolver(unsigned seed, MDDChart&& c)
:chart{c}, rgenerator{seed}, udistribution{0, c.num_elements()-1}
{   }

unsigned GreedyMDDSolver::select_random_element(void) noexcept {
    return udistribution(rgenerator);
}

unsigned GreedyMDDSolver::calc_distance_summatory_from_vertex_to_solution(unsigned v, const MDDSolution& solution) const noexcept {
    unsigned long sum = 0;
    for (const auto& u : solution.get_solution()) {
        sum += chart.at(v, u.first);
    }
    return sum;
}

unsigned GreedyMDDSolver::calc_vertex_candidate_dispersion(unsigned v, const MDDSolution& solution) const noexcept {
    unsigned long min = std::numeric_limits<unsigned long>::max();
    unsigned long max = std::numeric_limits<unsigned long>::min();
    unsigned long distance_summatory_vs = calc_distance_summatory_from_vertex_to_solution(v, solution);
    max = std::max(max, distance_summatory_vs);
    min = std::min(min, distance_summatory_vs);

    for (const auto& a : solution.get_solution()) {
        unsigned long sum = a.second + chart.at(v, a.first);
        max = std::max(max, sum);
        min = std::min(min, sum);
    }

    return max-min;
}

unsigned GreedyMDDSolver::select_next_element(const MDDSolution& solution, const std::set<unsigned>& nonchosen) const noexcept {
    unsigned next_element = *nonchosen.begin();
    unsigned long next_element_dispersion = std::numeric_limits<unsigned long>::max();

    for (const auto& a : nonchosen) {
        unsigned long dispersion = calc_vertex_candidate_dispersion(a, solution);
        if (dispersion < next_element_dispersion) {
            next_element = a;
            next_element_dispersion = dispersion;
        }
    }

    return next_element;
}

void GreedyMDDSolver::init_nonchosen(std::set<unsigned>& nonchosen) noexcept {
    nonchosen.clear();
    for (int i = 0; i < chart.num_elements(); i++) {
        nonchosen.insert(i);
    }
}

MDDSolution GreedyMDDSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
    MDDSolution solution;
    std::set<unsigned> nonchosen;
    init_nonchosen(nonchosen);

    unsigned first_element = select_random_element();
    solution.update(first_element, 0);
    nonchosen.erase(first_element);
    while (--number_of_elements_to_be_chosen) {
        unsigned element_chosen = select_next_element(solution, nonchosen);
        for (auto& v : solution.get_solution()) {
            solution.update(v.first, chart.at(element_chosen, v.first));
        }
        solution.update(element_chosen, calc_distance_summatory_from_vertex_to_solution(element_chosen, solution));
        nonchosen.erase(element_chosen);
    }

    return solution;
}