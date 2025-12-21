#include "genetic_algorithm.hpp"
#include "instance.hpp"
#include "solution.hpp"
#include "world_map.hpp"
#include <iostream>
#include <random>
#include <algorithm>
GeneticAlgorithm::GeneticAlgorithm(const Instance &i, int target_population_size) 
    : instance(i), target_population_size(target_population_size) {
  std::vector<int> pois = instance.getWorldMap().getPOIsIDs();
  std::random_device rd;
  std::mt19937 gen(rd());
  for (int i = 0; i < target_population_size; ++i) {
    std::shuffle(pois.begin(), pois.end(),gen);
    population.push_back(pois);
  }
}

std::vector<int> GeneticAlgorithm::findHostelsPath() {

  int give_up = 100;

  std::vector<int> hostels_path;

  // We need to count how many intermediate hostels we need.
  // We need the number of days - 1 intermediate hostels.
  int intermediate_hostel_count = instance.getDayCount() - 1;

  while (hostels_path.size() < intermediate_hostel_count) {
    std::cout << intermediate_hostel_count << " hostels needed, currently have "
              << hostels_path.size() << "\n";

    // Should we give up ?
    if (give_up <= 0) {
      std::cout
          << "Failed to find a valid hostels path after multiple tries.\n";
      break;
    }

    // Create a pool of available hostels to choose from.
    int total_hostels = instance.getWorldMap().getHostelCount();
    std::vector<int> available_hostels;
    for (int i = 0; i < total_hostels; ++i) {
      // Check if it's not the starting or ending hostel
      if (i == instance.getWorldMap().getStartingHostelIndex() ||
          i == instance.getWorldMap().getEndingHostelIndex()) {
        continue;
      }

      // Check if it's not already in the path
      // IMPORTANT : This check can be deleted to allow revisiting hostels,
      // Which is not againstance the problem's rules.
      if (std::find(hostels_path.begin(), hostels_path.end(), i) !=
          hostels_path.end()) {
        continue;
      }
      available_hostels.push_back(i);
    }

    // We now have a list of available hostels.
    // As long as we need more intermediate hostels, we will pick one at random
    // and add it to the path.

    // Filter the list of available hostels to only those reachable from the
    // last added hostel (or starting hostel if none added yet). If there are no
    // reachable hostels, we will remove all added hostels and try again.
    // After a set amount of attemps, we will give up.

    std::vector<int> reachable_hostels;
    int current_hostel_id = hostels_path.empty()
                                ? instance.getWorldMap().getStartingHostelIndex()
                                : hostels_path.back();
    for (int hostel_id : available_hostels) {
      // available_hostels and hostels_path are mutually exclusive by design.
      if (instance.getWorldMap().getDistanceBetweenPoints(
              instance.getWorldMap().getHostelByIndex(current_hostel_id),
              instance.getWorldMap().getHostelByIndex(hostel_id)) <=
          instance.getDayByIndex(static_cast<int>(hostels_path.size()))
              .getDuration()) {
        // Hostel is reachable
        reachable_hostels.push_back(hostel_id);
      }
    }
    // Is there at least one reachable hostel?
    if (reachable_hostels.empty()) {
      // Oops ! Remove all hostels added so far and try again.
      hostels_path.clear();
      --give_up;
      continue;
    }

    // Pick a random reachable hostel
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int random_index = std::rand() % reachable_hostels.size();
    int next_hostel_id = reachable_hostels[random_index];
    hostels_path.push_back(next_hostel_id);
  }

  return hostels_path;
}