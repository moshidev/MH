#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include "MDDChart.hpp"
#include "MDDSolution.hpp"
#include "GreedyMDDSolver.hpp" 
#include "LocalSearchMDDSolver.hpp" 

static std::vector<int> lee_semillas(const std::string& str) {
    std::vector<int> semillas;
    int num = 0;
    for (const auto& c : str) {
        if (c == ',') {
            semillas.push_back(num);
            num = 0;
        }
        else {
            num = num*10 + c-'0';
        }
    }
    semillas.push_back(num);
    return semillas;
}

int main(int argn, char** argv) {
    if (argn != 4) {
        std::cerr << "Utilización incorrecta. Invocar como " << argv[0] << " [tipo algoritmo] [semillas] [ficheros]\n";
        std::cerr << "Dónde:\n";
        std::cerr << "\t[tipo algoritmo] puede ser \"Greedy\" o \"LocalSearch\"\n";
        std::cerr << "\t[semilla] es una lista de semillas separada por comas (por ejemplo \"10,40,50,60,80\")\n";
        std::cerr << "\t[ficheros] es una lista de ficheros separados por comas\n";
        return 1;
    }

    std::vector<int> s = lee_semillas(std::string(argv[2]));

    for (auto n : s) {
        std::cout << ',' << n;
    }

    return 0;
}