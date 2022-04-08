#include <iostream>
#include "MDDChart.hpp"

MDDChart::MDDChart(unsigned n, unsigned m, const std::vector<int>& v) noexcept
:n{n}, m{m}, v{v}
{   }

MDDChart::MDDChart(unsigned n, unsigned m, std::vector<int>&& v) noexcept
:n{n}, m{m}, v{v}
{   }

MDDChart&& make_MDDChart(std::istream& is, unsigned res) noexcept {
    unsigned n, m;
    std::vector<int> v;
    is >> n >> m;

    if (is) {
        v.resize(n*n);
    }

    for (int i = 0; i < n; i++) {
        v[i*n+i] = 0;
    }
    
    while (is) {
        unsigned i, j;
        double dval;
        is >> i >> j >> dval;
        
        if (i >= n || j >= n || dval < 0.0) {
            is.setstate(std::ios_base::failbit);
        }
        if (is) {
            unsigned ival = dval*res;
            v[i*n+j] = ival;
            v[j*n+i] = ival;
        }
    }

    return std::move(MDDChart(n, m, std::move(v)));
}