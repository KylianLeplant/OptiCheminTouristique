#pragma once

#include <vector>

class Individual {
private:
public:
  // TODO : make this private for OOP
  std::vector<int> grand_tour;
  int fitness;
  Individual() = default;
  ~Individual() = default;
};