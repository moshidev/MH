#include <limits>
#include <vector>
#include <set>
#include "GreedyMDDSolver.hpp"

GreedyMDDSolver::GreedyMDDSolver(unsigned seed, const MDDChart& c)
:MDDSolver{seed, c}
{   }

GreedyMDDSolver::GreedyMDDSolver(unsigned seed, MDDChart&& c)
:MDDSolver{seed, c}
{   }

MDDSolution::sum_to_other_indexes_in_solution_t GreedyMDDSolver::calc_vertex_candidate_dispersion(MDDSolution::index_t v, const MDDSolution& solution) const noexcept {
    typedef MDDSolution::sum_to_other_indexes_in_solution_t sum_t;
    sum_t min = std::numeric_limits<sum_t>::max();
    sum_t max = std::numeric_limits<sum_t>::min();
    sum_t distance_summatory_vs = solution.calc_distance_summatory_from_vertex_to_solution(v, chart);
    max = std::max(max, distance_summatory_vs);
    min = std::min(min, distance_summatory_vs);

    for (const auto& a : solution.get_solution()) {
        sum_t sum = a.second + chart.at(v, a.first);
        max = std::max(max, sum);
        min = std::min(min, sum);
    }

    return max-min;
}

MDDSolution::index_t GreedyMDDSolver::select_next_element(const MDDSolution& solution, const std::set<MDDSolution::index_t>& nonchosen) const noexcept {
    typedef MDDSolution::sum_to_other_indexes_in_solution_t sum_t;
    MDDSolution::index_t next_element = *nonchosen.begin();
    sum_t next_element_dispersion = std::numeric_limits<sum_t>::max();

    for (const auto& a : nonchosen) {
        sum_t dispersion = calc_vertex_candidate_dispersion(a, solution);
        if (dispersion < next_element_dispersion) {
            next_element = a;
            next_element_dispersion = dispersion;
        }
    }

    return next_element;
}

void GreedyMDDSolver::init_nonchosen(std::set<MDDSolution::index_t>& nonchosen) noexcept {
    nonchosen.clear();
    for (int i = 0; i < chart.num_elements(); i++) {
        nonchosen.insert(i);
    }
}

MDDSolution GreedyMDDSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
    MDDSolution solution;
    std::set<MDDSolution::index_t> nonchosen;
    init_nonchosen(nonchosen);

    MDDSolution::index_t first_element = choose_random(nonchosen);
    solution.update(first_element, 0);
    nonchosen.erase(first_element);
    while (--number_of_elements_to_be_chosen) {
        MDDSolution::index_t element_chosen = select_next_element(solution, nonchosen);
        for (auto& v : solution.get_solution()) {
            solution.update(v.first, chart.at(element_chosen, v.first));
        }
        solution.update(element_chosen, solution.calc_distance_summatory_from_vertex_to_solution(element_chosen, chart));
        nonchosen.erase(element_chosen);
    }

    return solution;
}