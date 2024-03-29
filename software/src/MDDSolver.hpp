/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef MDD_SOLVER_HPP_
#define MDD_SOLVER_HPP_

#include <set>
#include <random>
#include <memory>
#include "MDDSolution.hpp"
#include "MDDChart.hpp"

class MDDSolver {
    std::mt19937 rgenerator;
    
protected:
    std::shared_ptr<const MDDChart> chart;

    MDDSolver() = delete;
    MDDSolver(const MDDSolver& g) = delete;
    MDDSolver(MDDSolver&& g) = delete;
    MDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c);

    void init_nonchosen(std::set<MDDSolution::index_t>& nonchosen) const noexcept;
    void init_nonchosen(std::set<MDDSolution::index_t>& nonchosen, const MDDSolution& sol) const noexcept;
    unsigned choose_random(const std::set<MDDSolution::index_t>& s) noexcept;
    void populate_randomly(MDDSolution& solution, std::set<MDDChart::index_t>& nonchosen, unsigned number_of_elements_to_be_chosen) noexcept;
    MDDSolution generate_random_solution(unsigned number_of_elements_to_be_chosen);
    int random_int(int i, int s) noexcept;

public:
    virtual ~MDDSolver() {}
    inline const MDDChart& get_chart(void) const noexcept { return *chart; }
    [[nodiscard]] virtual MDDSolution solve(unsigned number_of_elements_to_be_chosen) noexcept = 0;
};

#endif /* MDD_SOLVER_HPP_ */