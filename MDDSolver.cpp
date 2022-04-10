#include "MDDSolver.hpp"

MDDSolver::MDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c)
:rgenerator{seed}, chart{c}
{   }

void MDDSolver::init_nonchosen(std::set<MDDSolution::index_t>& nonchosen) const noexcept {
    nonchosen.clear();
    for (int i = 0; i < chart->num_elements(); i++) {
        nonchosen.insert(i);
    }
}

unsigned MDDSolver::choose_random(const std::set<MDDSolution::index_t>& s) noexcept {
    std::uniform_int_distribution<MDDSolution::index_t> distribution{0, static_cast<MDDSolution::index_t>(s.size()-1)};
    unsigned rpos = distribution(rgenerator);
    auto it = s.cbegin();
    std::advance(it, rpos);
    return *it;
}