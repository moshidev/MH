/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include "GeneticMDD.hpp"

GeneticMDD::GeneticMDD(unsigned seed, unsigned num_to_be_selected, float crossover_probability, float mutation_probability, crossover_func_t crossover_method, reemplace_func_t reemplace_method)
:rgenerator{seed}, crossover_method{crossover_method}, reemplace_method{reemplace_method}, num_to_be_selected{num_to_be_selected}, crossover_probability{crossover_probability}, mutation_probability{mutation_probability}
{	}

unsigned GeneticMDD::generation_step(population_t& p) {
	population_t selected = select(p, num_to_be_selected);
	unsigned num_eval = selected.size();
	crossover(selected, crossover_probability, crossover_method);
	mutate(selected, mutation_probability);
	reemplace_method(p, selected);
	return num_eval;
}

GeneticMDD::population_t GeneticMDD::select(const population_t& p, int num_to_be_selected) noexcept {
	// binary tournament selection
	population_t s;
	std::uniform_int_distribution<int> rand_dist{0, (int)p.size()-1};

    for (int i = 0; i < num_to_be_selected; i++) {
        int a = rand_dist(rgenerator);
        int b = rand_dist(rgenerator);
        s.push_back(p[a] < p[b] ? p[a] : p[b]);
    }

    return s;
}

void GeneticMDD::crossover(population_t& p, float probability, crossover_func_t f) noexcept {
	int crossovers_to_to = probability * p.size()/2;
    for (int i = 0; i < crossovers_to_to; i++) {
        auto pair{ f(this, p[i*2], p[i*2+1]) };
        p[i*2] = pair.first;
        p[i*2+1] = pair.second;
    }
}

void GeneticMDD::mutate(population_t& p, float probability) noexcept {
	unsigned n_gen = p.empty() ? 0 : p.size()*p.at(0).get_solution().size();
    unsigned n_mutations = probability * n_gen;
	std::uniform_int_distribution<int> rand_dist_chromosome{0, (int)p.size()-1};
	std::uniform_int_distribution<int> rand_dist_gen{0, p.empty() ? 0 : (int)p.front().get_solution().size()-1};
	std::uniform_int_distribution<int> rand_dist_mutation{0, p.empty() ? 0 : (int)p.front().get_chart().num_elements()-1};

    for (unsigned i = 0; i < n_mutations; i++) {
        unsigned chromosome = rand_dist_chromosome(rgenerator);
        auto& sol = p[chromosome];
        unsigned gen = rand_dist_gen(rgenerator);
        unsigned gen_index = sol.get_solution_vector()[gen];
        sol.remove_index_from_solution(gen_index);
        while (!sol.add_index_to_solution(rand_dist_mutation(rgenerator))) {   }   // warning: bogo
    }
}

void GeneticMDD::repair(MDDSolution& sol) noexcept {
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

void GeneticMDD::reemplace_generational(population_t& to_be_reemplaced, population_t& selected) noexcept {
	population_t selected_tmp{selected};
	MDDSolution best_solution{*find_best_solution_in_population(to_be_reemplaced)};
    to_be_reemplaced.swap(selected_tmp);
    to_be_reemplaced.erase(find_worst_solution_in_population(selected_tmp));
    to_be_reemplaced.push_back(best_solution);
}

void GeneticMDD::reemplace_stationary(population_t& to_be_reemplaced, population_t& selected) noexcept {
	for (int i = 0; i < selected.size(); i++) {
		auto worst_solution_it = find_worst_solution_in_population(to_be_reemplaced);
        if (selected[i].calc_dispersion() < worst_solution_it->calc_dispersion()) {
            to_be_reemplaced.erase(worst_solution_it);
            to_be_reemplaced.push_back(selected[i]);
        }
    }
}

std::pair<MDDSolution,MDDSolution> GeneticMDD::crossover_uniform(GeneticMDD* self, const MDDSolution& a, const MDDSolution& b) {
	auto chart = a.get_chart_pointer();
	std::uniform_int_distribution<int> rand_dist_fifty_fifty{0, 1};
	MDDSolution child[2] {{chart}, {chart}};
    auto va = a.get_solution_vector();
    auto vb = b.get_solution_vector();

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < (int)a.get_solution().size(); j++) {
            int r = rand_dist_fifty_fifty(self->rgenerator);
            child[i].add_index_to_solution(r == 0 ? va[j] : vb[j]);
        }
        self->repair(child[i]);
    }

    return {child[0], child[1]};
}

static std::vector<MDDSolution::index_t> get_distinct(const MDDSolution& sol_a, const MDDSolution& sol_b) {
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

std::pair<MDDSolution,MDDSolution> GeneticMDD::crossover_position(GeneticMDD* self, const MDDSolution& a, const MDDSolution& b) {
    MDDSolution intersection = a.intersect(b);
    MDDSolution child_a {intersection}, child_b {intersection};
    auto v_xor = get_distinct(a, b);
    std::shuffle(v_xor.begin(), v_xor.end(), self->rgenerator);

    const unsigned n_iters = v_xor.size()/2;
    for (int i = 0; i < n_iters; i++) {
        child_a.add_index_to_solution(v_xor.back());
        v_xor.pop_back();
        child_b.add_index_to_solution(v_xor.back());
        v_xor.pop_back();
    }

    return {child_a, child_b};
}

GeneticMDD::population_t GeneticMDD::generate_random_population(const std::shared_ptr<const MDDChart>& chart, int num_chromosomes, int num_genes) noexcept {
	population_t p;
	std::uniform_int_distribution<int> rand_dist{0, (int)chart->num_elements()-1};

    for (int i = 0; i < num_chromosomes; i++) {
        MDDSolution sol{chart};
        for (int j = 0; j < num_genes; j++) {
            while (!sol.add_index_to_solution(rand_dist(rgenerator))) {    }    // warning: bogo
        }
        p.push_back(sol);
    }

    return p;
}

GeneticMDD::population_t::iterator find_best_solution_in_population(GeneticMDD::population_t& p) noexcept {
	GeneticMDD::population_t::iterator best_solution_it = p.begin();
    unsigned best_solution_dispersion{std::numeric_limits<unsigned>::max()};

	auto it = p.begin();
    while (it != p.end()) {
        unsigned i_dispersion = it->calc_dispersion();
        if (i_dispersion < best_solution_dispersion) {
            best_solution_dispersion = i_dispersion;
            best_solution_it = it;
        }
		++it;
    }

    return best_solution_it;
}

GeneticMDD::population_t::iterator find_worst_solution_in_population(GeneticMDD::population_t& p) noexcept {
	GeneticMDD::population_t::iterator worst_solution_it = p.begin();
    unsigned worst_solution_dispersion{std::numeric_limits<unsigned>::min()};

	auto it = p.begin();
    while (it != p.end()) {
        unsigned i_dispersion = it->calc_dispersion();
        if (i_dispersion > worst_solution_dispersion) {
            worst_solution_dispersion = i_dispersion;
            worst_solution_it = it;
        }
		++it;
    }

    return worst_solution_it;
}