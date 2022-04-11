#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include "MDDChart.hpp"
#include "MDDSolution.hpp"
#include "GreedyMDDSolver.hpp" 
#include "LocalSearchMDDSolver.hpp" 

template<typename _T>
static std::vector<_T> read_list(std::string str, char delimiter);

int main(int argn, char** argv) {
    if (argn != 4) {
        std::cerr << "Utilización incorrecta. Invocar como " << argv[0] << " [tipo algoritmo] [semillas] [ficheros]\n";
        std::cerr << "Dónde:\n";
        std::cerr << "\t[tipo algoritmo] puede ser \"Greedy\" o \"LocalSearch\"\n";
        std::cerr << "\t[semilla] es una lista de semillas separada por comas (por ejemplo \"10,40,50,60,80\")\n";
        std::cerr << "\t[ficheros] es una lista de ficheros separados por comas\n";
        return 1;
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