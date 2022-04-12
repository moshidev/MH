/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include <iostream>
#include <fstream>
#include <cassert>
#include "MDDChart.hpp"
#include "MDDSolution.hpp"
#include "LocalSearchMDDSolver.hpp"

static bool correct_number_of_solutions(const MDDChart& chart) {
    LocalSearchMDDSolver solver{1, std::make_shared<const MDDChart>(chart)};
    MDDSolution s = solver.solve(chart.num_elements_to_be_chosen());
    size_t solution_size = s.get_solution().size();
    size_t m = chart.num_elements_to_be_chosen();
    for (auto a : s.get_solution()) {
        std::cerr << a.first << ", ";
    }
    std::cerr << std::endl;
    return solution_size == m && s.is_valid();
}


int main(int argn, char** argv) {
    std::fstream ifile{"data/GKD-b_50_n150_m45.txt", std::ios_base::in};
    const MDDChart chart = make_MDDChart(ifile);
    
    assert(correct_number_of_solutions(chart));

    std::cout << "All LocalSearchTests passed!!!" << std::endl;

    return 0;
}