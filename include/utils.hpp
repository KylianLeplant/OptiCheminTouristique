#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <algorithm>
#include "Solution.hpp"
#include "Instance.hpp"

// Fonction pour obtenir la liste de tous les POIs visités dans une solution
std::vector<int> getVisitedPOI(const Solution& solution);

// Fonction pour obtenir la liste de tous les hôtels visités dans une solution
std::vector<int> getVisitedHotel(const Solution& solution, Instance& instance);

// Fonction pour vérifier si un POI spécifique a été visité dans la solution
bool isPOIvisited(const Solution& solution, int poiID);

// Fonction pour vérifier si un hôtel spécifique a été visité dans la solution
bool isHotelVisited(const Solution& solution, int hotelID);

// Fonction pour ajouter un POI à la solution
void appendPOI(Instance& instance, Solution& solution, int poiID, int atIndex = -1);

// Fonction pour ajouter un hôtel intermédiaire à la solution
void appendHotel(Solution& solution, int hotelID);

// Fonction pour comparer deux solutions
bool equals(const Solution& sol1, const Solution& sol2);

#endif