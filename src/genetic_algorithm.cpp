#include "genetic_algorithm.hpp"
#include "instance.hpp"
#include "solution.hpp"
#include "world_map.hpp"

GeneticAlgorithm::GeneticAlgorithm(const Instance &i) : inst(i) {
  target_population_size = 1;
  population.push_back(Solution());
}

std::vector<int> GeneticAlgorithm::findHostelsPath() {

  std::vector<int> hostels_path;
  int inbetween_days_count = inst.getDayCount() - 1;

  while (hostels_path.size() < inbetween_days_count) {
    int current_hostel_id = hostels_path.empty()
                                ? inst.getWorldMap().getStartingHostelIndex()
                                : hostels_path.back();
    int next_hostel_id = -1;

    // Compute reachable hostels from current hostel
    std::vector<int> reachable_hostels;
    int total_hostels = inst.getWorldMap().getHostelCount();
    int remaining_days_count = inbetween_days_count - hostels_path.size();

    for (int hostel_id = 0; hostel_id < total_hostels; ++hostel_id) {
      // Avoid the same hostel and check distance
      if (hostel_id != current_hostel_id &&
          std::find(hostels_path.begin(), hostels_path.end(), hostel_id) ==
              hostels_path.end()) {
        float distance = inst.getWorldMap().getDistanceBetweenPoints(
            inst.getWorldMap().getHostelByIndex(current_hostel_id),
            inst.getWorldMap().getHostelByIndex(hostel_id));
        // hostels_path.size() represents the current day (0-indexed)
        int current_day = static_cast<int>(hostels_path.size());
        float max_travel = inst.getDayByIndex(current_day).getDuration();
        if (distance <= max_travel) {
          reachable_hostels.push_back(hostel_id);
        }
      }
    }

    // Choose a random hostel among the reachable ones
    if (!reachable_hostels.empty()) {
      std::srand(std::time(nullptr));
      next_hostel_id =
          reachable_hostels[std::rand() % reachable_hostels.size()];
      hostels_path.push_back(next_hostel_id);
    } else {
      // No reachable hostel, remove the last one if possible
      if (!hostels_path.empty()) {
        hostels_path.pop_back();
      } else {
        break; // Impossible to build a path}
      }
    }
  }
  return hostels_path;
}