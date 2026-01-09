#pragma once

#include "individual.hpp"
#include "instance.hpp"
#include "solution.hpp"
#include <random>
#include <vector>

class Solver {
private:
  std::mt19937 rng;
  Instance inst;

public:
  Solver(const Instance &instance, int seed);
  ~Solver() = default;

  /**
   * Calcualte the optimal hostel to stay at between two POIs given the current
   * day time.
   * We look at the last POI of day N and the first POi of day N+1 and pick
   * the hostel that minimizes the specific detour required to bridge them.
   */
  int selectBestHostel(int last_poi_id, int next_poi_id, float current_day_time,
                       float max_day_duration) const;

  /**
   * Converts a grand tour into a solution by splitting it into days and adding
   * hostels.
   */
  Solution decode(const std::vector<int> &grand_tour) const;

  /**
   * Operator : Order Crossover (OX1)
   */
  Individual crossover(const Individual &parent1, const Individual &parent2);

  /**
   * Operator : 2-Opt (Inversion) & Swap
   */
  void mutate(Individual &indiv);

  /**
   * Local Search : Intensification by swapping two POIs in the grand tour several times
   */
  void intensifyBySwap(Individual &indiv, int iterations);

  /**
   * Main solving function.
   * Spends up to time_limit seconds to find a good solution.
   * @param time_limit in seconds
   * @param population_size
   * @param elitism_rate the fraction of best individuals to keep. Must be
   * between 0 and 1.
   * @param tournament_size the pool of contestants for tourmament selection.
   * Must be smaller than population_size.
   * @param mutation_rate probability for a mutation
   */
  Solution solve(int time_limit, int population_size, float elitism_rate,
                 int tournament_size, float mutation_rate, float intensify_rate, int intensify_iterations, float intensify_generation_probability);
};