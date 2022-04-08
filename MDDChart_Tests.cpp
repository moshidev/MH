#include <cassert>
#include <fstream>
#include <vector>
#include "MDDChart.hpp"

static bool make_chart(const std::string& filename) {
    std::fstream ifile{filename, std::ios_base::in};
    MDDChart w{make_MDDChart(ifile)};
    return !ifile.bad();
}

int main(int argn, char** argv) {
    assert(make_chart("data/GKD-b_1_n25_m2.txt"));
    
    std::cout << "All WorldData_Tests passed!" << std::endl;
    return 0;
}