#include <limits>
#include <set>
#include "LocalSearchMDDSolver.hpp"

LocalSearchMDDSolver::LocalSearchMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& chart)
:MDDSolver{seed, chart}
{   }

void LocalSearchMDDSolver::populate_randomly(MDDSolution& solution, std::set<MDDChart::index_t>& nonchosen, unsigned number_of_elements_to_be_chosen) noexcept {
    for (int i = 0; i < number_of_elements_to_be_chosen; i++) {
        MDDChart::index_t selected = choose_random(nonchosen);
        nonchosen.erase(selected);
        solution.add_index_to_solution(selected);
    }
}

MDDChart::index_t LocalSearchMDDSolver::best_first_neighbour_from_index(MDDChart::index_t index, const MDDSolution& solution, const std::set<MDDChart::index_t>& nonchosen) const noexcept {
    typedef MDDSolution::sum_to_other_indexes_in_solution_t sum_t;
    typedef MDDSolution::index_t index_t;
    MDDSolution s_tmp{solution};
    index_t best_index = index;
    sum_t best_index_dispersion = solution.calc_dispersion();

    s_tmp.remove_index_from_solution(index);
    for (const auto& a : nonchosen) {
        sum_t dispersion = s_tmp.calc_index_candidate_dispersion(a);
        if (dispersion < best_index_dispersion) {
            best_index = a;
            return best_index;
        }
    }

    return best_index;
}

MDDSolution LocalSearchMDDSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
    typedef MDDSolution::sum_to_other_indexes_in_solution_t sum_t;
    MDDSolution solution{chart};
    std::set<MDDChart::index_t> nonchosen;
    init_nonchosen(nonchosen);
    populate_randomly(solution, nonchosen, number_of_elements_to_be_chosen);
    sum_t best_dispersion = solution.calc_dispersion();
    sum_t dispersion = std::numeric_limits<sum_t>::max();

    int i = 0;
    while (best_dispersion < dispersion && i < 100'000) {
        MDDSolution s_tmp{solution};
        dispersion = best_dispersion;
        for (const auto& a : s_tmp.get_solution()) {
            MDDSolution::index_t new_index = best_first_neighbour_from_index(a.first, solution, nonchosen);
            nonchosen.insert(a.first);
            nonchosen.erase(new_index);
            solution.remove_index_from_solution(a.first);
            solution.add_index_to_solution(new_index);
            best_dispersion = solution.calc_dispersion();
        }
        i++;
    }

    return solution;
}