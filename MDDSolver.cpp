#include "MDDSolver.hpp"

MDDSolver::MDDSolver(unsigned seed, const MDDChart& c)
:rgenerator{seed}, chart{c}
{   }

MDDSolver::MDDSolver(unsigned seed, MDDChart&& c)
:rgenerator{seed}, chart{c}
{   }

unsigned MDDSolver::choose_random(const std::set<MDDSolution::index_t>& s) noexcept {
    std::uniform_int_distribution<MDDSolution::index_t> distribution{0, static_cast<MDDSolution::index_t>(s.size()-1)};
    unsigned rpos = distribution(rgenerator);
    auto it = s.cbegin();
    std::advance(it, rpos);
    return *it;
}