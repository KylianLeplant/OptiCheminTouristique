#ifndef GENETIC_HPP
#define GENETIC_HPP

#include "Solution.hpp"
#include "Instance.hpp"
#include <vector>
#include <memory>
#include <limits>
#include <ctime>


class GeneticAlgorithm
{
private:
    std::vector<Solution> population;
    Instance* instance;
    int populationSize;
    //float mutationRate;
public:
    GeneticAlgorithm(Instance* inst);

    // Méthodes pour l'algorithme génétique
    std::vector<int> findHotelsPath();
    std::vector<int> getReachableHotels(int solutionID,int currentDay);
    int remainingTime(int solutionID, int dayIndex = -1);
    //retourne l'index du jour actuel (index) de la solution 
    int getCurrentDay(int solutionID);
    //retourne le nombre de jours restants à partir de la solution, n'inclue pas le jour actuel
    int remainingDays(int solutionID);
    
    void initializePopulation();
    void evaluateFitness();
    void selection();
    void crossover();
    void mutation();
    Solution getBestSolution();
};

#endif