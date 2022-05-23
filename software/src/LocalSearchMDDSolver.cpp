/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include <limits>
#include <set>
#include "LocalSearchMDDSolver.hpp"

LocalSearchMDDSolver::LocalSearchMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& chart)
:MDDSolver{seed, chart}
{   }

std::pair<MDDChart::index_t,unsigned> LocalSearchMDDSolver::best_first_neighbour_from_index(MDDChart::index_t index, const MDDSolution& solution, const std::set<MDDChart::index_t>& nonchosen) const noexcept {
    typedef MDDSolution::sum_to_other_indexes_in_solution_t sum_t;
    typedef MDDSolution::index_t index_t;
    MDDSolution s_tmp{solution};
    index_t best_index = index;
    sum_t best_index_dispersion = solution.calc_dispersion();

    unsigned n_evals = 0;
    s_tmp.remove_index_from_solution(index);
    for (const auto& a : nonchosen) {
        ++n_evals;
        sum_t dispersion = s_tmp.calc_index_candidate_dispersion(a);
        if (dispersion < best_index_dispersion) {
            best_index = a;
            best_index_dispersion = dispersion;
            return {best_index,n_evals};
        }
    }

    return {best_index,n_evals};
}

std::pair<MDDSolution,unsigned> LocalSearchMDDSolver::local_search(const MDDSolution& ini, unsigned number_of_elements_to_be_chosen, unsigned max_num_of_neighbour_evals) {
    typedef MDDSolution::sum_to_other_indexes_in_solution_t sum_t;
    std::set<MDDChart::index_t> nonchosen;
    init_nonchosen(nonchosen, ini);
    MDDSolution solution{ini};
    sum_t best_dispersion = solution.calc_dispersion();
    sum_t dispersion = std::numeric_limits<sum_t>::max();

    int i = 0;
    int num_neighbour_evals = 0;
    while (best_dispersion < dispersion && i < 100'000 && num_neighbour_evals < max_num_of_neighbour_evals) {
        MDDSolution s_tmp{solution};
        dispersion = best_dispersion;
        for (const auto& a : s_tmp.get_solution()) {
            auto res = best_first_neighbour_from_index(a.first, solution, nonchosen);
            num_neighbour_evals += res.second;
            MDDSolution::index_t new_index = res.first;
            if (a.first != new_index) {
                nonchosen.insert(a.first);
                nonchosen.erase(new_index);
                solution.remove_index_from_solution(a.first);
                solution.add_index_to_solution(new_index);
                best_dispersion = solution.calc_dispersion();
            }
        }
        i++;
    }

    return {solution,i};
}

MDDSolution LocalSearchMDDSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
    MDDSolution solution_ini{chart};
    std::set<MDDChart::index_t> nonchosen;
    init_nonchosen(nonchosen);
    populate_randomly(solution_ini, nonchosen, number_of_elements_to_be_chosen);
    return local_search(solution_ini, number_of_elements_to_be_chosen, std::numeric_limits<unsigned>::max()).first;
}