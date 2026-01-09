#pragma once

#include <vector>

/**
 * Represents one individual in the population for the Genetic Algorithm.
 * Fields are public for simplicity and direct access, avoiding function call
 * overhead.
 */
class Individual {
private:
public:
  std::vector<int> grand_tour;
  int fitness;
  Individual() = default;
  ~Individual() = default;
};