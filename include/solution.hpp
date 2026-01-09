#pragma once

#include "instance.hpp"
#include <vector>

// Represents one possible solution for a problem instance.
class Solution {
public:
  std::vector<int> intermediate_hostels;
  std::vector<std::vector<int>> pois_sequence;
  std::vector<float> start_dates;
  int score_value;

  Solution();
  virtual ~Solution();

  bool isValid(const Instance &inst) const;
};