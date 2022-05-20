/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include <algorithm>
#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "MDDChart.hpp"
#include "MDDSolution.hpp"
#include "GreedyMDDSolver.hpp" 
#include "LocalSearchMDDSolver.hpp" 
#include "GeneticAGG_MDDSolver.hpp"
#include "GeneticAGE_MDDSolver.hpp"

template<typename _T>
static std::vector<_T> read_list(std::string str, char delimiter);

static std::vector<std::pair<std::string,std::shared_ptr<MDDChart>>> read_charts(const std::vector<std::string>& v, unsigned resolution);

int main(int argn, char** argv) {
    if (argn != 4) {
        std::cerr << "Utilización incorrecta. Invocar como " << argv[0] << " [tipo algoritmo] [semillas] [ficheros]\n";
        std::cerr << "Dónde:\n";
        std::cerr << "\t[tipo algoritmo] puede ser \"Greedy\" o \"LocalSearch\"\n";
        std::cerr << "\t[semilla] es una lista de semillas separada por comas (por ejemplo \"10,40,50,60,80\")\n";
        std::cerr << "\t[ficheros] es una lista de ficheros separados por comas\n";
        return 1;
    }

    std::string tipo_algoritmo{argv[1]};
    std::transform(tipo_algoritmo.begin(), tipo_algoritmo.end(), tipo_algoritmo.begin(), [](char c){ return std::tolower(c); });
    if (tipo_algoritmo != "greedy" && tipo_algoritmo != "localsearch" && tipo_algoritmo != "genetic_agg_uniform" && tipo_algoritmo != "genetic_agg_position" && tipo_algoritmo != "genetic_age_uniform" && tipo_algoritmo != "genetic_age_position") {
        std::cerr << "Tipo de algoritmo desconocido." << std::endl;
        return 1;
    }

    char delimiter = ',';
    unsigned resolution = 100'000;
    auto seeds = read_list<int>(argv[2], delimiter);
    auto files = read_list<std::string>(argv[3], delimiter);
    auto charts = read_charts(files, resolution);

    if (seeds.empty() || files.empty()) {
        std::cerr << "Abortamos, no hay ni semillas ni ficheros..." << std::endl;
        return 2;
    }

    for (const auto& c : charts) {
        std::cout << c.first;

        double elapsed_time = 0.0;
        for (const auto& s : seeds) {
            std::unique_ptr<MDDSolver> solver;
            if (tipo_algoritmo == "greedy") {
                solver = std::make_unique<GreedyMDDSolver>(s, c.second);
            }
            else if (tipo_algoritmo == "localsearch") {
                solver = std::make_unique<LocalSearchMDDSolver>(s, c.second);
            }
            else if (tipo_algoritmo == "genetic_agg_uniform") {
                solver = std::make_unique<GeneticAGG_MDDSolver>(s, c.second, 50, 0.7, 0.1);
                GeneticAGG_MDDSolver* uniform = (GeneticAGG_MDDSolver*)solver.get();
                uniform->set_crossover_method(uniform->crossover_uniform);
            }
            else if (tipo_algoritmo == "genetic_agg_position") {
                solver = std::make_unique<GeneticAGG_MDDSolver>(s, c.second, 50, 0.7, 0.1);
                GeneticAGG_MDDSolver* position = (GeneticAGG_MDDSolver*)solver.get();
                position->set_crossover_method(position->crossover_position);
            }
            else if (tipo_algoritmo == "genetic_age_uniform") {
                solver = std::make_unique<GeneticAGE_MDDSolver>(s, c.second, 50, 0.1);
                GeneticAGE_MDDSolver* uniform = (GeneticAGE_MDDSolver*)solver.get();
                uniform->set_crossover_method(uniform->crossover_uniform);
            }
            else if (tipo_algoritmo == "genetic_age_position") {
                solver = std::make_unique<GeneticAGE_MDDSolver>(s, c.second, 50, 0.1);
                GeneticAGE_MDDSolver* position = (GeneticAGE_MDDSolver*)solver.get();
                position->set_crossover_method(position->crossover_position);
            }
            auto ini = std::chrono::high_resolution_clock::now();
            auto solution = solver->solve(c.second->num_elements_to_be_chosen());
            auto fin = std::chrono::high_resolution_clock::now();
            elapsed_time += std::chrono::duration<double>(fin - ini).count();
            std::cout << '\t' << (double)solution.calc_dispersion()/resolution;
        }
        
        std::cout << '\t' << elapsed_time/seeds.size() << '\n';
    }

    return 0;
}

template<typename _T>
static std::vector<_T> read_list(std::string str, char delimiter) {
    std::replace(str.begin(), str.end(), delimiter, ' ');
    std::stringstream sstream{str};
    std::vector<_T> ret;

    while (sstream) {
        _T val;
        sstream >> val;
        if (sstream) {
            ret.push_back(val);
        }
    }
    
    return ret;
}

static std::vector<std::pair<std::string,std::shared_ptr<MDDChart>>> read_charts(const std::vector<std::string>& v, unsigned resolution) {
    std::vector<std::pair<std::string,std::shared_ptr<MDDChart>>> charts;

    for (const auto& e : v) {
        std::fstream file{e};
        MDDChart chart{make_MDDChart(file, resolution)};
        if (!file.bad()) {
            charts.emplace_back(e, std::make_shared<MDDChart>(chart));
        }
        else {
            std::cerr << "Invalid data file " << e << std::endl;
        }
    }

    return charts;
}