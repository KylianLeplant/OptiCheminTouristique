#pragma once

#include "instance.hpp"
#include <vector>

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

  Solution();
  virtual ~Solution();
  bool isValid(const Instance &instance);
};