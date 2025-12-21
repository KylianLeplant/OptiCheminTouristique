#pragma once

#include "instance.hpp"
#include <vector>
#include <set>
#include "world_map.hpp"
#include <iostream>
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
  Solution(const Instance& instance, const std::vector<int>& genome);
  virtual ~Solution();

  // verifies if the solution is valid according to problem constraints
  bool isValid() const;

  // returns the set of visited hotels' IDs
  std::set<int> getVisitedHotels() const;

  // returns the set of visited POIs' IDs
  std::set<int> getVisitedPOIs() const;

  // checks if a specific POI has been visited in the solution
  bool isPOIVisited(int poi_id) const;

  // calculates the total visit duration for a specific day
  float getDayVisitsDuration(int day) const;
  
  int findNearestHostel(Point target_point, const std::set<int>& excluded_hostels) const;
  
};