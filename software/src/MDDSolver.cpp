/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include "MDDSolver.hpp"

MDDSolver::MDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c)
:rgenerator{seed}, chart{c}
{   }

void MDDSolver::init_nonchosen(std::set<MDDSolution::index_t>& nonchosen) const noexcept {
    nonchosen.clear();
    for (unsigned i = 0; i < chart->num_elements(); i++) {
        nonchosen.insert(i);
    }
}

void MDDSolver::init_nonchosen(std::set<MDDSolution::index_t>& nonchosen, const MDDSolution& sol) const noexcept {
    init_nonchosen(nonchosen);
    for (const auto& a : sol.get_solution()) {
        nonchosen.erase(a.first);
    }
}

unsigned MDDSolver::choose_random(const std::set<MDDSolution::index_t>& s) noexcept {
    std::uniform_int_distribution<MDDSolution::index_t> distribution{0, static_cast<MDDSolution::index_t>(s.size()-1)};
    unsigned rpos = distribution(rgenerator);
    auto it = s.cbegin();
    std::advance(it, rpos);
    return *it;
}

void MDDSolver::populate_randomly(MDDSolution& solution, std::set<MDDChart::index_t>& nonchosen, unsigned number_of_elements_to_be_chosen) noexcept {
    for (unsigned i = 0; i < number_of_elements_to_be_chosen; i++) {
        MDDChart::index_t selected = choose_random(nonchosen);
        nonchosen.erase(selected);
        solution.add_index_to_solution(selected);
    }
}

MDDSolution MDDSolver::generate_random_solution(unsigned number_of_elements_to_be_chosen) {
	MDDSolution random_solution{chart};
	std::set<MDDSolution::index_t> nonchosen;
	init_nonchosen(nonchosen);
	populate_randomly(random_solution, nonchosen, number_of_elements_to_be_chosen);
	
	return random_solution;
}

int MDDSolver::random_int(int i, int s) noexcept {
    std::uniform_int_distribution<int> distribution{i, s};
    return distribution(rgenerator);
}