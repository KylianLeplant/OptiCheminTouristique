#pragma once

#include "instance.hpp"
#include "solution.hpp"
#include <ctime>
#include <vector>

class GeneticAlgorithm {
private:
  std::vector<Solution> population;
  const Instance &inst;
  int target_population_size;

  std::vector<int> getReachableHostels();
  int getRemainingTime(int solutionID, int dayIndex);
  int getCurrentDay(int solutionID);
  int getRemainingDays(int solutionID);

  void initializePopulation();
  void evaluateFitness();
  void selection();
  void crossover();
  void mutation();

public:
  GeneticAlgorithm(const Instance &inst);

  std::vector<int> findHostelsPath();
  Solution getBestSolution();

public:
};