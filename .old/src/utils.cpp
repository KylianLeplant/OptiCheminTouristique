#ifndef UTILS_HPP
#define UTILS_HPP

#include "utils.hpp"
#include "Solution.hpp"
#include <vector>
#include <algorithm>


// Fonction pour obtenir la liste de tous les POIs visités dans une solution
std::vector<int> getVisitedPOI(const Solution& solution)
{
    std::vector<int> visitedPOIs;

    for (std::vector<int> dailyRoute : solution.v_v_Sequence_Id_Par_Jour)
    {
        visitedPOIs.insert(visitedPOIs.end(), dailyRoute.begin(), dailyRoute.end()); //ajout des POIs visités chaque jour
    }

    return visitedPOIs;

}

// Fonction pour obtenir la liste de tous les hôtels visités dans une solution
std::vector<int> getVisitedHotels(const Solution& solution, Instance& instance)
{
    std::vector<int> visitedHotels = solution.v_Id_Hotel_Intermedaire;
    visitedHotels.insert(visitedHotels.begin(), instance.get_Id_Hotel_depart()); // Ajouter l'hôtel de départ (id 0)
    visitedHotels.push_back(instance.get_Id_Hotel_Arrivee()); // Ajouter l'hôtel d'arrivée (dernier id)

    return visitedHotels;
}

// Fonction pour vérifier si un POI spécifique a été visité dans la solution
bool isPOIvisited(const Solution& solution, int poiID)
{
    std::vector<int> visitedPOIs = getVisitedPOI(solution);
    return std::find(visitedPOIs.begin(), visitedPOIs.end(), poiID) != visitedPOIs.end();
}

// Fonction pour vérifier si un hôtel spécifique a été visité dans la solution
bool isHotelVisited(const Solution& solution, Instance& instance, int hotelID)
{
    std::vector<int> visitedHotels = getVisitedHotels(solution, instance);
    return std::find(visitedHotels.begin(), visitedHotels.end(), hotelID) != visitedHotels.end();
}

// Fonction pour ajouter un POI à la solution
void appendPOI(Instance& instance, Solution& solution, int poiID, int atIndex = -1)
{
    // Si pas de jours dans la solution, rien à faire
    if (solution.v_v_Sequence_Id_Par_Jour.empty()) {
        return;
    }

    // Dernier jour (index non signé)
    std::size_t dayIndex = solution.v_v_Sequence_Id_Par_Jour.size() - 1;
    auto &dayRoute = solution.v_v_Sequence_Id_Par_Jour[dayIndex];

    // Calculer une position sûre dans [0, dayRoute.size()]
    std::size_t pos;
    if (atIndex < 0) {
        // convention : atIndex < 0 -> append en fin
        pos = dayRoute.size();
    } else {
        std::size_t requested = static_cast<std::size_t>(atIndex);
        pos = (requested > dayRoute.size()) ? dayRoute.size() : requested; // clamp
    }

    dayRoute.insert(dayRoute.begin() + pos, poiID);
    solution.i_valeur_fonction_objectif += instance.get_POI_Score(poiID);
}


// Fonction pour ajouter un hôtel intermédiaire à la solution
void appendHotel(Solution& solution, int hotelID)
{
    solution.v_Id_Hotel_Intermedaire.push_back(hotelID);
}

// Fonction pour comparer deux solutions
bool equals(const Solution& sol1, const Solution& sol2)
{
    for (std::size_t day = 0; day < sol1.v_v_Sequence_Id_Par_Jour.size(); ++day)
    {
        const std::vector<int>& route1 = sol1.v_v_Sequence_Id_Par_Jour[day];
        const std::vector<int>& route2 = sol2.v_v_Sequence_Id_Par_Jour[day];

        if (route1.size() != route2.size())
            return false;

        if (day!= 0)
        {
            if (sol1.v_Id_Hotel_Intermedaire[day - 1] != sol2.v_Id_Hotel_Intermedaire[day - 1])
                return false;
        }

        for (std::size_t i = 0; i < route1.size(); ++i)
        {
            if (route1[i] != route2[i])
                return false;
        }
    }

    return true;
}
#endif