#pragma once

#include "instance.hpp"
#include <vector>

// Represents one possible solution for a problem instance.
// Everything is public for simplicity and direct access, avoiding function call
// overhead.
class Solution {
public:
  std::vector<int> intermediate_hostels;
  std::vector<std::vector<int>> pois_sequence;
  std::vector<float> start_dates;
  int score_value;
  double resolution_time;

  Solution();
  virtual ~Solution();

  bool isValid(const Instance &inst, bool verbose = false) const;
};