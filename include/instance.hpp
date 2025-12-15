#pragma once

#include "day.hpp"
#include "world_map.hpp"
#include <string>
#include <vector>

// Represents an instance of the problem. In other words, it loads a data file
// and extracts a map, as well as other problem inputs.
class Instance {
private:
  std::string file_path; // The file this instance was loaded from
  WorldMap world_map;    // The map that contains the hostels and POIs
  std::vector<Day> days; // The duration of each day in the trip

  float max_exploration_hours; // The maximum exploration hours allowed

public:
  Instance(const std::string &file_path);
  ~Instance() = default;

  const WorldMap &getWorldMap() const; // Returns a copy of the world map
  int getDayCount() const;
  const Day &getDayByIndex(const int index) const;
};