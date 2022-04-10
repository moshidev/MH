#include "MDDSolver.hpp"

MDDSolver::MDDSolver(unsigned seed, const MDDChart& c)
:rgenerator{seed}, chart{c}
{   }

MDDSolver::MDDSolver(unsigned seed, MDDChart&& c)
:rgenerator{seed}, chart{c}
{   }

unsigned MDDSolver::choose_random(const std::set<unsigned>& s) noexcept {
    std::uniform_int_distribution<std::uint_fast32_t> distribution{0, static_cast<std::uint_fast32_t>(s.size()-1)};
    unsigned rpos = distribution(rgenerator);
    auto it = s.cbegin();
    std::advance(it, rpos);
    return *it;
}