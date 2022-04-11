#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include "GreedyMDDSolver.hpp"
#include "MDDChart.hpp"
#include "MDDSolution.hpp"

static bool correct_number_of_solutions(const MDDChart& chart) {
    GreedyMDDSolver solver{1, std::make_shared<const MDDChart>(chart)};
    MDDSolution s = solver.solve(chart.num_elements_to_be_chosen());
    size_t solution_size = s.get_solution().size();
    size_t m = chart.num_elements_to_be_chosen();
    for (auto a : s.get_solution()) {
        std::cerr << a.first << ", ";
    }
    std::cerr << std::endl;
    return solution_size == m && s.is_valid();
}

static bool correct_dispersion(const MDDChart& chart, unsigned dispersion) {
    GreedyMDDSolver solver{1, std::make_shared<const MDDChart>(chart)};
    MDDSolution s = solver.solve(chart.num_elements_to_be_chosen());
    unsigned sdispersion = s.calc_dispersion();
    std::cerr << "Dispersion: " << sdispersion << std::endl;
    return sdispersion == dispersion;
}

int main(int argn, char** argv) {
    std::fstream ifile{"data/GKD-b_50_n150_m45.txt", std::ios_base::in};
    const MDDChart chart = make_MDDChart(ifile);

    assert(correct_number_of_solutions(chart));
    assert(correct_dispersion(chart, 4674332));

    std::cout << "All tests passed!!" << std::endl;

    return 0;
}