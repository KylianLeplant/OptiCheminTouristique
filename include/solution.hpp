#pragma once

#include "instance.hpp"
#include <vector>
#include <set>

// Represents one possible solution for a problem instance.
class Solution {
public:
  /**
   * Intermediate hostels IDs matching their position in the worldmap vector.
   */
  std::vector<int> intermediate_hostels;

  /**
   * POI visit sequence per day. Outer vector index is the day number, inner
   * vector contains POI IDs visited that day.
   * IDs correspond to their position in the worldmap vector.
   */

  std::vector<std::vector<int>> pois_sequence;

  /**
   * Start date/time for each day in the solution.
   */
  std::vector<float> start_dates;
  int score_value;

  const Instance& instance;

  Solution() = delete;
  Solution(const Instance& instance);
  virtual ~Solution();
  bool isValid() const;
  std::set<int> getVisitedHotels() const;
  std::set<int> getVisitedPOIs() const;
  bool isPOIVisited(int poi_id) const;
  
};