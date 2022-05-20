/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#ifndef GENETIC_MDD_HPP_
#define GENETIC_MDD_HPP_

#include <random>
#include <functional>
#include "MDDChart.hpp"
#include "MDDSolution.hpp"

class GeneticMDD {
public:
	typedef std::vector<MDDSolution> population_t;
	typedef std::function<std::pair<MDDSolution,MDDSolution>(GeneticMDD*,const MDDSolution&,const MDDSolution&)> crossover_func_t;
	typedef std::function<void(population_t&,population_t&)> reemplace_func_t;

protected:
	std::mt19937 rgenerator;
	crossover_func_t crossover_method;
	reemplace_func_t reemplace_method;
	unsigned num_to_be_selected;
	float crossover_probability;
    float mutation_probability;

	virtual population_t select(const population_t& p, int num_to_be_selected) noexcept;	// by binary tournament
	virtual void crossover(population_t& p, float probability, crossover_func_t f) noexcept;
    virtual void mutate(population_t& p, float probability) noexcept;
    virtual void repair(MDDSolution& sol) noexcept;
	
public:
	GeneticMDD(unsigned seed, unsigned num_to_be_selected, float crossover_probability, float mutation_probability, crossover_func_t crossover_method, reemplace_func_t reemplace_method);

	static std::pair<MDDSolution,MDDSolution> crossover_uniform(GeneticMDD* self, const MDDSolution& a, const MDDSolution& b);
    static std::pair<MDDSolution,MDDSolution> crossover_position(GeneticMDD* self, const MDDSolution& a, const MDDSolution& b);

	static void reemplace_generational(population_t& to_be_reemplaced, population_t& selected) noexcept;
	static void reemplace_stationary(population_t& to_be_reemplaced, population_t& selected) noexcept;

	inline void set_num_to_be_selected(unsigned num) noexcept { num_to_be_selected = num; }
	inline void set_crossover_probability(float prob) noexcept { crossover_probability = prob; }
	inline void set_mutation_probability(float prob) noexcept { mutation_probability = prob; }
	inline void set_crossover_method(crossover_func_t crossover_method) noexcept { this->crossover_method = crossover_method; }
	
	population_t generate_random_population(const std::shared_ptr<const MDDChart>& chart, int num_chromosomes, int num_genes) noexcept;
	unsigned generation_step(population_t& p);	// returns the number of evals of the objective function.
};

GeneticMDD::population_t::iterator find_best_solution_in_population(GeneticMDD::population_t& p) noexcept;
GeneticMDD::population_t::iterator find_worst_solution_in_population(GeneticMDD::population_t& p) noexcept;

#endif /* GENETIC_MDD_HPP_ */