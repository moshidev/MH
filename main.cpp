#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <set>
#include "MDDChart.hpp"
#include "GreedyMDDSolver.hpp" 

int main(int argn, char** argv) {

    std::set<std::filesystem::path> data_files;
    for (const auto& f : std::filesystem::directory_iterator("data")) {
        if (f.is_regular_file()) {
            data_files.insert(f.path());
        }
    }

    for (const auto& path : data_files) {
        double elapsed_time = 0.0;
        std::cout << path;

        std::fstream ifile{path.string(), std::ios_base::in};
        MDDChart chart{make_MDDChart(ifile)};
        for (unsigned i = 0; i < 10; i++) {
            GreedyMDDSolver solver{i, chart};
            auto ini = std::chrono::high_resolution_clock::now();
            std::set<unsigned> solution {solver.solve(chart.num_elements_to_be_chosen())};
            auto fin = std::chrono::high_resolution_clock::now();
            elapsed_time += std::chrono::duration<double>(fin - ini).count();

            std::cout << '\t' << ((double)solver.calc_solution_dispersion(solution)) / 10000;
        }
        std::cout << '\t' << elapsed_time/10 << "s" << std::endl;
    }
}