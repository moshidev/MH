#ifndef MDD_SOLVER_HPP_
#define MDD_SOLVER_HPP_

#include <set>
#include <random>
#include "MDDSolution.hpp"
#include "MDDChart.hpp"

class MDDSolver {
    std::mt19937 rgenerator;

protected:
    MDDChart chart;

    MDDSolver() = delete;
    MDDSolver(const MDDSolver& g) = delete;
    MDDSolver(MDDSolver&& g) = delete;
    MDDSolver(unsigned seed, const MDDChart& c);
    MDDSolver(unsigned seed, MDDChart&& c);

    unsigned choose_random(const std::set<MDDSolution::index_t>& s) noexcept;

public:
    inline const MDDChart& get_chart(void) const noexcept { return chart; }
};

#endif /* MDD_SOLVER_HPP_ */