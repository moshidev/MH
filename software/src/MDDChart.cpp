/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include <iostream>
#include "MDDChart.hpp"

MDDChart::MDDChart(unsigned n, unsigned m, const std::vector<index_t>& v) noexcept
:n{n}, m{m}, v{v}
{   }

MDDChart::MDDChart(unsigned n, unsigned m, std::vector<index_t>&& v) noexcept
:n{n}, m{m}, v{v}
{   }

MDDChart make_MDDChart(std::istream& is, unsigned res) noexcept {
    unsigned n, m;
    std::vector<MDDChart::index_t> v;
    is >> n >> m;
    
    if (is) {
        v.resize(n*n);
        for (unsigned i = 0; i < n; i++) {
            v[i*n+i] = 0;
        }
    }
    
    unsigned n_lineas = (n*(n-1))/2;
    unsigned n_lineas_leidas = 0;
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
            n_lineas_leidas++;
        }
    }

    if (n_lineas_leidas != n_lineas) {
        is.setstate(std::ios_base::failbit);
    }

    return MDDChart(n, m, v);
}