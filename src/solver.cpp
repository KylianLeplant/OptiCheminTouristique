#include "solver.hpp"
#include "individual.hpp"
#include "instance.hpp"
#include "solution.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

Solver::Solver(const Instance &instance, int seed)
    : inst(instance), rng(seed) {}

int Solver::selectBestHostel(int last_poi_id, int next_poi_id,
                             float current_day_time,
                             float max_day_duration) const {
  int best_hostel = -1;
  float best_added_dist = std::numeric_limits<float>::max();

  for (int hostel_id = 0; hostel_id < inst.getHostelCount(); ++hostel_id) {
    float dist_to_hostel = inst.getDistanceHostelPOI(hostel_id, last_poi_id);

    if (current_day_time + dist_to_hostel <= max_day_duration) {
      float dist_from_hostel =
          (next_poi_id >= 0) ? inst.getDistanceHostelPOI(hostel_id, next_poi_id)
                             : 0.0f;

      float total_cost = dist_to_hostel + dist_from_hostel;

      if (total_cost < best_added_dist) {
        best_added_dist = total_cost;
        best_hostel = hostel_id;
      }
    }
  }
  return best_hostel;
}

Solution Solver::decode(const std::vector<int> &grand_tour) const {
  Solution sol;
  sol.score_value = 0;
  sol.pois_sequence.resize(inst.getDayCount());
  sol.start_dates.resize(inst.getDayCount(), 0.0f);

  // --- CHANGED: Use a mask to track visited POIs instead of a linear index ---
  std::vector<char> visited(grand_tour.size(), 0);

  int current_start_hostel = inst.getStartingHostelID();

  for (int day = 0; day < inst.getDayCount(); ++day) {
    float current_time = 0.0f;
    float max_duration = inst.getDayDuration(day);

    int current_poi_id = -1;
    int current_hostel_id = current_start_hostel;

    // --- CHANGED: Iterate through the WHOLE tour for every day ---
    // This implements the "Sieve": if a POI doesn't fit, we skip it (continue)
    // and try the next one, filling the day as tightly as possible.
    for (size_t i = 0; i < grand_tour.size(); ++i) {

      if (visited[i])
        continue; // Skip POIs already assigned to previous days/slots

      int candidate_poi = grand_tour[i];

      // A. Calculate travel time
      float dist = 0.0f;
      if (current_poi_id == -1) {
        dist = inst.getDistanceHostelPOI(current_hostel_id, candidate_poi);
      } else {
        dist = inst.getDistancePOIPOI(current_poi_id, candidate_poi);
      }

      float arrival = current_time + dist;
      float opening = inst.getPOIOpeningTime(candidate_poi);
      float closing = inst.getPOIClosingTime(candidate_poi);

      // B. Handle Time Windows
      if (arrival < opening) {
        arrival = opening;
      }

      // C. Check Validity
      bool possible = true;

      if (arrival > closing) {
        possible = false;
      }

      if (possible) {
        if (day == inst.getDayCount() - 1) {
          // Last Day : Must reach Ending Hostel
          float dist_end = inst.getDistanceHostelPOI(inst.getEndingHostelID(),
                                                     candidate_poi);
          if (arrival + dist_end > max_duration) {
            possible = false;
          }
        } else {
          // Normal Day : Must reach ANY hostel
          // Optimization: check against the closest hostel.
          float min_dist_hostel = std::numeric_limits<float>::max();
          for (int h = 0; h < inst.getHostelCount(); ++h) {
            float d = inst.getDistanceHostelPOI(h, candidate_poi);
            if (d < min_dist_hostel)
              min_dist_hostel = d;
          }
          if (arrival + min_dist_hostel > max_duration) {
            possible = false;
          }
        }
      }

      if (possible) {
        // D. Commit the move
        sol.pois_sequence[day].push_back(candidate_poi);
        sol.score_value += (int)inst.getPOIScore(candidate_poi);
        current_time = arrival;
        current_poi_id = candidate_poi;
        current_hostel_id = -1;

        visited[i] = 1; // Mark as used so we don't visit it again later
      }
      // E. ELSE: Do nothing! (This is the "continue" logic)
      // We just loop to i+1 and try to fit the NEXT candidate into the
      // remaining time.
    }

    // 2. End of Day : Choose intermediate Hostel
    if (day < inst.getDayCount() - 1) {

      // Look ahead: Find the very first unvisited POI in the grand tour
      // to guide us in the right direction.
      int next_poi_target = -1;
      for (size_t k = 0; k < grand_tour.size(); ++k) {
        if (!visited[k]) {
          next_poi_target = grand_tour[k];
          break;
        }
      }

      int last_poi_visited =
          (sol.pois_sequence[day].empty()) ? -1 : sol.pois_sequence[day].back();

      int chosen_hostel = -1;

      if (last_poi_visited == -1) {
        chosen_hostel = current_start_hostel;
      } else {
        chosen_hostel = selectBestHostel(last_poi_visited, next_poi_target,
                                         current_time, max_duration);
      }

      if (chosen_hostel == -1) {
        chosen_hostel = 0;
      }

      sol.intermediate_hostels.push_back(chosen_hostel);
      current_start_hostel = chosen_hostel;
    }
  }

  return sol;
}

Individual Solver::crossover(const Individual &p1, const Individual &p2) {
  Individual child;
  child.grand_tour.resize(p1.grand_tour.size());

  int size = (int)p1.grand_tour.size();
  int start = std::uniform_int_distribution<int>(0, size - 2)(rng);
  int end = std::uniform_int_distribution<int>(start + 1, size - 1)(rng);

  std::vector<bool> taken(inst.getPOICount(), false);
  for (int i = start; i <= end; ++i) {
    child.grand_tour[i] = p1.grand_tour[i];
    taken[p1.grand_tour[i]] = true;
  }

  int current_p2_idx = 0;
  for (int i = 0; i < size; ++i) {
    if (i >= start && i <= end) {
      continue;
    }
    while (taken[p2.grand_tour[current_p2_idx]]) {
      current_p2_idx++;
    }
    child.grand_tour[i] = p2.grand_tour[current_p2_idx];
    taken[p2.grand_tour[current_p2_idx]] = true;
  }

  return child;
}

void Solver::mutate(Individual &ind) {
  int size = (int)ind.grand_tour.size();
  if (size < 2)
    return;

  std::uniform_int_distribution<int> dist(0, size - 1);
  int type = dist(rng) % 2;
  int i = dist(rng);
  int j = dist(rng);
  while (i == j) {
    j = dist(rng);
  }

  if (type == 0) {
    std::swap(ind.grand_tour[i], ind.grand_tour[j]);
  } else {
    if (i > j)
      std::swap(i, j);
    std::reverse(ind.grand_tour.begin() + i, ind.grand_tour.begin() + j + 1);
  }
}

Solution Solver::solve(int time_limit, int population_size, float elitism_rate,
                       int tournament_size, float mutation_rate) {
  auto start_time = std::chrono::high_resolution_clock::now();

  std::vector<Individual> population(population_size);
  std::vector<int> base_tour(inst.getPOICount());
  for (int i = 0; i < inst.getPOICount(); ++i) {
    base_tour[i] = i;
  }

  for (auto &ind : population) {
    ind.grand_tour = base_tour;
    std::shuffle(ind.grand_tour.begin(), ind.grand_tour.end(), rng);
    Solution s = decode(ind.grand_tour);
    ind.fitness = s.score_value;
  }

  int generation = 0;

  while (true) {
    auto now = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - start_time)
            .count() >= time_limit) {
      break;
    }

    std::sort(population.begin(), population.end(),
              [](const Individual &a, const Individual &b) {
                return a.fitness > b.fitness;
              });

    std::vector<Individual> next_gen;

    int elites = (int)(population_size * elitism_rate);
    for (int i = 0; i < elites; ++i) {
      next_gen.push_back(population[i]);
    }

    while (next_gen.size() < population_size) {
      int best_idx = -1;
      int best_fit = -1;
      for (int k = 0; k < tournament_size; ++k) {
        int idx =
            std::uniform_int_distribution<int>(0, population_size / 2)(rng);
        if (population[idx].fitness > best_fit) {
          best_fit = population[idx].fitness;
          best_idx = idx;
        }
      }
      Individual &p1 = population[best_idx];
      Individual &p2 = population[std::uniform_int_distribution<int>(
          0, population_size - 1)(rng)];

      Individual child = crossover(p1, p2);

      if (std::uniform_real_distribution<float>(0, 1)(rng) < mutation_rate) {
        mutate(child);
      }

      Solution s = decode(child.grand_tour);
      child.fitness = s.score_value;
      next_gen.push_back(child);
    }
    population = next_gen;
    generation++;

    if (generation % 1000 == 0) {
      int best = population[0].fitness;
      float average = std::accumulate(population.begin(), population.end(), 0,
                                      [](int sum, const Individual &ind) {
                                        return sum + ind.fitness;
                                      }) /
                      (float)population.size();
      std::cout << "Generation " << generation << " | Best Score: " << best
                << " | Average Score: " << average << " | Time: "
                << std::chrono::duration_cast<std::chrono::seconds>(now -
                                                                    start_time)
                       .count()
                << "s" << std::endl;
    }
  }

  std::sort(population.begin(), population.end(),
            [](const Individual &a, const Individual &b) {
              return a.fitness > b.fitness;
            });

  std::cout << "Final Best Score: " << population[0].fitness << std::endl;
  return decode(population[0].grand_tour);
}