/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef MDD_CHART_HPP_
#define MDD_CHART_HPP_

#include <iostream>
#include <vector>

class MDDChart {
public:
    typedef unsigned index_t;

private:
    const unsigned n, m;
    const std::vector<index_t> v;

public:
    MDDChart() = delete;
    MDDChart(const MDDChart& c) = default;
    MDDChart(MDDChart&& c) = default;
    MDDChart(unsigned n, unsigned m, const std::vector<index_t>& v) noexcept;
    MDDChart(unsigned n, unsigned m, std::vector<index_t>&& v) noexcept;

    inline unsigned num_elements(void) const noexcept { return n; }
    inline unsigned num_elements_to_be_chosen(void) const noexcept { return m; }
    
    inline index_t at(unsigned i, unsigned j) const noexcept { return v[i*n+j]; }
};

[[nodiscard]] MDDChart make_MDDChart(std::istream& is, unsigned res = 10'000) noexcept;

#endif /* MDD_CHART_HPP_ */