/**
 * Daniel Pedrosa Montes © 2022
 * Expat (MIT) License.
 */

#include "SimulatedAnnealingMDDSolver.hpp"
#include <cmath>
#include <set>

SimulatedAnnealingMDDSolver::SimulatedAnnealingMDDSolver(unsigned seed, const std::shared_ptr<const MDDChart>& c, unsigned max_num_eval, unsigned max_num_neighbours, unsigned max_num_successes, float final_temp)
:MDDSolver{seed, c}, rgen{seed}, max_num_eval{max_num_eval}, max_num_neighbours{max_num_neighbours}, max_num_successes{max_num_successes}, final_temp{final_temp}
{   }

float SimulatedAnnealingMDDSolver::cauchy_annealing_policy(float initial_temperature, float last_temperature, unsigned total_num_coolings_we_will_be_doing) const noexcept {
    float beta = (initial_temperature - final_temp)/(total_num_coolings_we_will_be_doing*initial_temperature*final_temp);
    return last_temperature/(1 + beta*last_temperature);
}

float SimulatedAnnealingMDDSolver::calc_initial_temp(float micro, float initial_sol_cost, float fi) const noexcept {
    return (micro*initial_sol_cost)/(-std::log(fi));
}

MDDSolution SimulatedAnnealingMDDSolver::make_random_neighbour_from_solution(const MDDSolution& s) noexcept {
    MDDSolution random_neighbour{s};
    auto v = s.get_solution_vector();
    std::uniform_int_distribution<int> rand_dist_vector(0, v.size()-1);
    std::uniform_int_distribution<int> rand_dist_chart(0, s.get_chart().num_elements()-1);

    random_neighbour.remove_index_from_solution(v[rand_dist_vector(rgen)]);
    while (!random_neighbour.add_index_to_solution(rand_dist_chart(rgen))) {   }   // warning: bogo

    return random_neighbour;
}

bool SimulatedAnnealingMDDSolver::random_acceptance(float delta_f, float temperature) noexcept {
    std::uniform_real_distribution<float> rand_dist_zero_to_one(0.0, 1.0);
    float divisor = boltzmann_constant*temperature;
    float exp = 1/std::exp(delta_f/divisor);
    return rand_dist_zero_to_one(rgen) < exp;
}

MDDSolution SimulatedAnnealingMDDSolver::solve(unsigned number_of_elements_to_be_chosen) noexcept {
    MDDSolution solution{chart};
    std::set<MDDSolution::index_t> nonchosen;
    init_nonchosen(nonchosen);
    populate_randomly(solution, nonchosen, number_of_elements_to_be_chosen);
    
    float initial_temperature = calc_initial_temp(0.3, solution.calc_dispersion(), 0.3);
    float temperature = initial_temperature;
    MDDSolution best_solution{solution};
    
    unsigned total_num_coolings_we_will_be_doing = max_num_eval/max_num_neighbours;
    unsigned num_eval = 0;
    unsigned num_improvements = 1;
    while (temperature > final_temp && num_eval < max_num_eval) {// && num_improvements > 0) {
        unsigned neighbours_generated = 0;
        unsigned neighbours_accepted = 0;
        num_improvements = 0;
        while (neighbours_generated < max_num_neighbours && neighbours_accepted < max_num_successes) {
            MDDSolution random_neighbour {make_random_neighbour_from_solution(solution)};
            num_eval++; neighbours_generated++;
            int delta_f = random_neighbour.calc_dispersion() - (int)solution.calc_dispersion();
            if (delta_f < 0 || random_acceptance(delta_f, temperature)) {
                neighbours_accepted++;
                solution = random_neighbour;
                if (solution.calc_dispersion() < best_solution.calc_dispersion()) {
                    num_improvements++;
                    best_solution = solution;
                }
            }
        }

        temperature = cauchy_annealing_policy(initial_temperature, temperature, total_num_coolings_we_will_be_doing);
    }


    return best_solution;
}