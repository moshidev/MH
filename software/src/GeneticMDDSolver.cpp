/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include <random>
#include "GeneticMDDSolver.hpp"

GeneticMDDSolver::GeneticMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c)
:MDDSolver{seed, c}, num_eval{0}
{   }

GeneticMDDSolver::population_t GeneticMDDSolver::generate_random_population(int num_chromosomes, int num_genes) noexcept {
    population_t p;

    for (int i = 0; i < num_chromosomes; i++) {
        MDDSolution sol{chart};
        for (int j = 0; j < num_genes; j++) {
            while (!sol.add_index_to_solution(random_int(0, chart->num_elements()-1))) {    }    // warning: bogo
        }
        p.push_back(sol);
    }

    return p;
}

GeneticMDDSolver::population_t GeneticMDDSolver::select_by_binary_tournament(const population_t& p, int num_to_be_selected) noexcept {
    population_t s;

    for (int i = 0; i < num_to_be_selected; i++) {
        int a = random_int(0, p.size()-1);
        int b = random_int(0, p.size()-1);
        s.push_back(p[a] < p[b] ? p[a] : p[b]);
    }

    return s;
}

void GeneticMDDSolver::crossover(population_t& p, float probability, crossover_func_t f) noexcept {
    int crossovers_to_to = probability * p.size()/2;
    for (int i = 0; i < crossovers_to_to; i++) {
        auto pair{ f(this, p[i*2], p[i*2+1]) };
        p[i*2] = pair.first;
        p[i*2+1] = pair.second;
    }
}

void GeneticMDDSolver::mutate(population_t& p, float probability) noexcept {
    unsigned n_gen = p.empty() ? 0 : p.size()*p.at(0).get_solution().size();
    unsigned n_mutations = probability * n_gen;
    for (unsigned i = 0; i < n_mutations; i++) {
        unsigned chromosome = random_int(0, p.size()-1);
        auto& sol = p[chromosome];
        unsigned gen = random_int(0, sol.get_solution().size()-1);
        unsigned gen_index = sol.get_solution_vector()[gen];
        sol.remove_index_from_solution(gen_index);
        while (!sol.add_index_to_solution(random_int(0, sol.get_chart().num_elements()-1))) {   }   // warning: bogo
    }
}

std::pair<MDDSolution,MDDSolution> GeneticMDDSolver::crossover_uniform(GeneticMDDSolver* self, const MDDSolution& a, const MDDSolution& b) {
    MDDSolution child[2] {{self->chart}, {self->chart}};
    auto va = a.get_solution_vector();
    auto vb = b.get_solution_vector();

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < (int)a.get_solution().size(); j++) {
            int r = self->random_int(0, 1);
            child[i].add_index_to_solution(r == 0 ? va[j] : vb[j]);
        }
        self->repair(child[i]);
    }

    return {child[0], child[1]};
}

std::vector<MDDSolution::index_t> get_distinct(const MDDSolution& sol_a, const MDDSolution& sol_b) {
    std::vector<MDDSolution::index_t> distinct;

    for (const auto& a : sol_a.get_solution()) {
        if (sol_b.get_solution().find(a.first) == sol_b.get_solution().end()) {
            distinct.push_back(a.first);
        }
    }
    for (const auto& b : sol_b.get_solution()) {
        if (sol_a.get_solution().find(b.first) == sol_a.get_solution().end()) {
            distinct.push_back(b.first);
        }
    }

    return distinct;
}

std::pair<MDDSolution,MDDSolution> GeneticMDDSolver::crossover_position(GeneticMDDSolver* self, const MDDSolution& a, const MDDSolution& b) {
    std::mt19937 random_generator{static_cast<std::mt19937::result_type>(self->random_int(0, std::numeric_limits<int>::max()))};
    MDDSolution intersection = a.intersect(b);
    MDDSolution child_a {intersection}, child_b {intersection};
    auto v_xor = get_distinct(a, b);
    std::shuffle(v_xor.begin(), v_xor.end(), random_generator);

    const unsigned n_iters = v_xor.size()/2;
    for (int i = 0; i < n_iters; i++) {
        child_a.add_index_to_solution(v_xor.back());
        v_xor.pop_back();
        child_b.add_index_to_solution(v_xor.back());
        v_xor.pop_back();
    }

    return {child_a, child_b};
}

void GeneticMDDSolver::repair(MDDSolution& sol) noexcept {
    typedef MDDSolution::sum_to_other_indexes_in_solution_t sum_t;
    typedef MDDSolution::index_t index_t;
    while (sol.get_solution().size() > sol.get_chart().num_elements_to_be_chosen()) {
        index_t to_remove {sol.get_solution().begin()->first};
        sum_t to_remove_dispersion = std::numeric_limits<sum_t>::min();
        for (auto& a : sol.get_solution()) {
            index_t index = a.first;

            if (sol.remove_index_from_solution(index)) {
                sum_t dispersion = sol.calc_dispersion();
                sol.add_index_to_solution(index);

                if (dispersion > to_remove_dispersion) {
                    to_remove = index;
                    to_remove_dispersion = dispersion;
                }
            }
        }
        sol.remove_index_from_solution(to_remove);
    }
    while (sol.get_solution().size() < sol.get_chart().num_elements_to_be_chosen()) {
        index_t to_add {sol.get_solution().begin()->first};
        sum_t to_add_dispersion = std::numeric_limits<sum_t>::max();
        for (int i = 0; i < (int)sol.get_chart().num_elements(); i++) {
            index_t index = i;

            if (sol.add_index_to_solution(index)) {
                sum_t dispersion = sol.calc_dispersion();
                sol.remove_index_from_solution(index);

                if (dispersion < to_add_dispersion) {
                    to_add = index;
                    to_add_dispersion = dispersion;
                }
            }
        }
        sol.add_index_to_solution(to_add);
    }
}

std::vector<bool> GeneticMDDSolver::get_binary_representation_from(const MDDSolution& sol) const noexcept {
    std::vector<bool> v(sol.get_solution().size(), false);

    for (const auto& a : sol.get_solution()) {
        v[a.first] = true;
    }
 
    return v;
}

unsigned GeneticMDDSolver::find_best_solution_pos_in_population(const population_t& p) noexcept {
    unsigned best_solution_pos = 0;
    unsigned best_solution_dispersion{std::numeric_limits<unsigned>::max()};

    for (int i = 0; i < (int)p.size(); i++) {
        unsigned i_dispersion = p[i].calc_dispersion();
        if (i_dispersion < best_solution_dispersion) {
            best_solution_dispersion = i_dispersion;
            best_solution_pos = i;
        }
    }

    return best_solution_pos;
}

unsigned GeneticMDDSolver::find_worst_solution_pos_in_population(const population_t& p) noexcept {
    unsigned worst_solution_pos = 0;
    unsigned worst_solution_dispersion{std::numeric_limits<unsigned>::min()};

    for (int i = 0; i < (int)p.size(); i++) {
        unsigned i_dispersion = p[i].calc_dispersion();
        if (i_dispersion > worst_solution_dispersion) {
            worst_solution_dispersion = i_dispersion;
            worst_solution_pos = i;
        }
    }

    return worst_solution_pos;
}