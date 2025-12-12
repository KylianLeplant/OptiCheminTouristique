#include "Genetic.hpp"
#include <algorithm>
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm(Instance* inst)
{
    // Initialisation des paramètres de l'algorithme génétique
    populationSize = 1;
    //mutationRate = 0.01f;
    instance = inst;
    population.push_back(Solution());
}

std::vector<int> GeneticAlgorithm::findHotelsPath()
{
    std::vector<int> hotelsPath;
    
    // Vérification que l'instance est valide
    if (instance == nullptr) {
        std::cerr << "Erreur: instance est null" << std::endl;
        return hotelsPath;
    }
    
    int nbJoursIntermediaires = instance->get_Nombre_Jour() - 1;  // Nombre d'hôtels intermédiaires à trouver
    
    if (nbJoursIntermediaires <= 0) {
        std::cerr << "Erreur: nombre de jours invalide (" << instance->get_Nombre_Jour() << ")" << std::endl;
        return hotelsPath;
    }
    
    while (hotelsPath.size() < nbJoursIntermediaires)
    {
        int currentHotelID = hotelsPath.empty() ? instance->get_Id_Hotel_depart() : hotelsPath.back();
        int nextHotelID = -1;
        
        // Calculer les hôtels accessibles depuis l'hôtel actuel
        std::vector<int> reachableHotels;
        int totalHotels = instance->get_Nombre_Hotel();
        int remainingDaysCount = nbJoursIntermediaires - hotelsPath.size();
        
        // Vérification que currentHotelID est valide
        if (currentHotelID < 0 || currentHotelID >= totalHotels) {
            std::cerr << "Erreur: ID hôtel invalide (" << currentHotelID << ")" << std::endl;
            break;
        }
        
        for (int hotelID = 0; hotelID < totalHotels; ++hotelID)
        {
            // Éviter le même hôtel et vérifier la distance
            if (hotelID != currentHotelID && 
                std::find(hotelsPath.begin(), hotelsPath.end(), hotelID) == hotelsPath.end())
            {
                float distance = instance->get_distance_Hotel_Hotel(currentHotelID, hotelID);
                // hotelsPath.size() représente le jour actuel (0-indexed)
                int currentDay = static_cast<int>(hotelsPath.size());
                if (currentDay >= instance->get_Nombre_Jour()) {
                    std::cerr << "Erreur: index jour hors limites (" << currentDay << ")" << std::endl;
                    break;
                }
                float maxTravel = instance->get_POI_Duree_Max_Voyage(currentDay);
                if (distance <= maxTravel)
                {
                    reachableHotels.push_back(hotelID);
                }
            }
        }

        // Choix d'un hôtel aléatoire parmi les accessibles
        if (!reachableHotels.empty())
        {
            std::srand(std::time(nullptr));
            nextHotelID = reachableHotels[std::rand() % reachableHotels.size()];
            hotelsPath.push_back(nextHotelID);
        }
        else
        {
            // Aucun hôtel accessible, retirer le dernier si possible
            if (!hotelsPath.empty())
            {
                hotelsPath.pop_back();
            }
            else
            {
                break; // Impossible de construire un chemin
            }
        }
    }

    return hotelsPath;
}

std::vector<int> GeneticAlgorithm::getReachableHotels(int solutionID, int currentDay)
{
    int currentHotelID = population[solutionID].v_Id_Hotel_Intermedaire[currentDay];
    std::vector<int> reachableHotels;
    int totalHotels = instance->get_Nombre_Hotel();

    for (int hotelID = 0; hotelID < totalHotels; ++hotelID)
    {
        if (hotelID != currentHotelID && instance->get_distance_Hotel_Hotel(currentHotelID, hotelID) <= instance->get_POI_Duree_Max_Voyage(this->remainingDays(solutionID)-1))
        {
            reachableHotels.push_back(hotelID);
        }
    }

    return reachableHotels;
}

int GeneticAlgorithm::remainingTime(int solutionID, int dayIndex)
{
    Solution& solution = population[solutionID];
    //TODO: implement
    return 0;
}

int GeneticAlgorithm::getCurrentDay(int solutionID)
{
    Solution& solution = population[solutionID];
    return solution.v_Id_Hotel_Intermedaire.size()-1;
}

int GeneticAlgorithm::remainingDays(int solutionID)
{
    Solution& solution = population[solutionID];
    return instance->get_Nombre_Jour() - solution.v_Id_Hotel_Intermedaire.size();
}