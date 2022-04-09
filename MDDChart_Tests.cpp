#include <cassert>
#include <fstream>
#include <vector>
#include "MDDChart.hpp"

static bool make_chart(const std::string& filename) {
    std::fstream ifile{filename, std::ios_base::in};
    MDDChart w{make_MDDChart(ifile)};
    return !ifile.bad();
}

static bool read_chart(const std::string& filename, unsigned n, unsigned m) {
    std::fstream ifile{filename, std::ios_base::in};
    MDDChart w{make_MDDChart(ifile)};
    if (ifile.bad() || n != w.num_elements() || m != w.num_elements_to_be_chosen()) {
        return false;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            volatile unsigned a = w.at(i, j);
            a = a + 1;
        }
    }
    
    return true;
}

int main(int argn, char** argv) {
    assert(make_chart("data/GKD-b_1_n25_m2.txt"));
    assert(read_chart("data/GKD-b_50_n150_m45.txt", 150, 45));
    
    std::cout << "All WorldData_Tests passed!" << std::endl;
    return 0;
}