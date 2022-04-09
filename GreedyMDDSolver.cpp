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

unsigned long GreedyMDDSolver::calc_distance_summatory_from_vertex_to_solution(unsigned v, const std::set<unsigned>& solution) const noexcept {
    unsigned long sum = 0;
    for (unsigned u : solution) {
        sum += chart.at(v, u);
    }
    return sum;
}

unsigned long GreedyMDDSolver::calc_solution_dispersion(const std::set<unsigned>& solution) const noexcept {
    unsigned long min = std::numeric_limits<unsigned long>::max();
    unsigned long max = std::numeric_limits<unsigned long>::min();

    for (const auto& v : solution) {
        unsigned long sum = calc_distance_summatory_from_vertex_to_solution(v, solution);
        max = std::max(max, sum);
        min = std::min(min, sum);
    }

    return max-min;
}

unsigned GreedyMDDSolver::select_next_element(const std::set<unsigned>& solution, const std::set<unsigned>& nonchosen) const noexcept {
    std::set<unsigned> posible_solution{solution};
    unsigned next_element = *nonchosen.begin();
    unsigned long next_element_dispersion = std::numeric_limits<unsigned long>::max();
    

    for (const auto& a : nonchosen) {
        posible_solution.insert(a);
        unsigned long dispersion = calc_solution_dispersion(posible_solution);
        if (dispersion < next_element_dispersion) {
            next_element = a;
            next_element_dispersion = dispersion;
        }
        posible_solution.erase(a);
    }

    return next_element;
}

void GreedyMDDSolver::init_nonchosen(std::set<unsigned>& nonchosen) noexcept {
    nonchosen.clear();
    for (int i = 0; i < chart.num_elements(); i++) {
        nonchosen.insert(i);
    }
}

std::set<unsigned> GreedyMDDSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
    std::set<unsigned> solution;
    std::set<unsigned> nonchosen;
    init_nonchosen(nonchosen);

    unsigned first_element = select_random_element();
    solution.insert(first_element);
    nonchosen.erase(first_element);
    while (--number_of_elements_to_be_chosen) {
        unsigned element_chosen = select_next_element(solution, nonchosen);
        solution.insert(element_chosen);
        nonchosen.erase(element_chosen);
    }

    return solution;
}