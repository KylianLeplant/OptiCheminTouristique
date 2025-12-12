#define CHEMIN_DOSSIER_DONNEES "Data/"
#define NOM_FICHIER_LISTE_FICHIER_DONNEES "data.txt"
#define NOM_FICHIER_LISTE_SORTIE "sortie.txt"

#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "Instance.hpp"
#include "Solution.hpp"
#include "utils.hpp"
#include "Genetic.hpp"

using namespace std;

int Resolution(Instance * instance);
 

int main(int argc, const char * argv[])
{ 
    // Diagnostic immédiat avec printf (plus bas niveau que cout)
    printf("=== DEBUT DU PROGRAMME ===\n");
    fflush(stdout);
    
    // Diagnostic: utiliser stdio pour éviter d'utiliser iostream au tout début
    //std::cout << "Début du programme de résolution d'instances.\n" << std::flush;
    try
    {
        printf("Avant ouverture fichiers\n");
        fflush(stdout);
        //std::cout << "Programme de résolution d'instances démarré." << std::endl;
        string s_tmp;
        string s_chemin=CHEMIN_DOSSIER_DONNEES;
        s_chemin.append(NOM_FICHIER_LISTE_FICHIER_DONNEES);
        
        printf("Chemin: %s\n", s_chemin.c_str());
        fflush(stdout);
        
        ifstream fichier(s_chemin.c_str(), std::ios::in);std::ofstream fichier_Sortie_Resume;
        s_chemin=CHEMIN_DOSSIER_DONNEES;
        s_chemin.append(NOM_FICHIER_LISTE_SORTIE);
        ofstream fichier_Sortie(s_chemin.c_str(), std::ios::out | std::ios::app);
        if(fichier)
        {
            if(fichier_Sortie)
            {
                fichier_Sortie<<" Fichier données\t Tps de résolution \t Best solution"<<endl;
                getline(fichier,s_tmp);
                while(s_tmp!="")
                {
                    Instance * instance = new Instance();
                    chrono::time_point<chrono::system_clock> chrono_start, chrono_end;
                    chrono::duration<double> elapsed;
                    int i_best_solution_score=0;
                    s_chemin=CHEMIN_DOSSIER_DONNEES;
                    std::cout<< " Résolution de "<<s_tmp<<endl;
                    s_chemin.append(s_tmp);
                    s_chemin.erase(remove(s_chemin.begin(), s_chemin.end(), '\r'), s_chemin.end());
                    s_chemin.erase(remove(s_chemin.begin(), s_chemin.end(), '\n'), s_chemin.end());
                    
                    instance->chargement_Instance(s_chemin);
                    chrono_start = chrono::system_clock::now();
                    i_best_solution_score=Resolution(instance);
                    cout<< " Fin de résolution de "<<s_tmp<<endl;
                    chrono_end = chrono::system_clock::now();
                    elapsed=chrono_end-chrono_start;
                    fichier_Sortie<<s_chemin <<"\t"<<elapsed.count()<<"\t"<< i_best_solution_score <<endl;
                    s_tmp="";
                    getline(fichier,s_tmp);
                    delete instance;
                }
                fichier_Sortie.close();
            }
            else
            {
                cout<<" Erreur lecture des données : chemin vers la sortie non valide. "<<endl;
            }
            fichier.close();
        }
        else
        {
            cout<<" Erreur lecture des données : chemin listant l'ensemble des données non valide. "<<endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception std::exception : " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Exception inconnue levée" << std::endl;
        return 1;
    }
    return 0;
}

int Resolution(Instance * instance)
{
    // Vérification que l'instance est valide
    if (instance == nullptr) {
        std::cerr << "Erreur: instance est null dans Resolution()" << std::endl;
        return -1;
    }
    
    std::cout << "Début de la résolution avec l'algorithme génétique..." << std::endl;
    std::cout << "Instance chargée: " << instance->get_Nombre_Hotel() << " hôtels, " 
              << instance->get_Nombre_POI() << " POIs, " 
              << instance->get_Nombre_Jour() << " jours" << std::endl;
    
    GeneticAlgorithm ga(instance);
    std::cout << "Algorithme génétique créé, recherche du chemin..." << std::endl;
    
    std::vector<int> hotelsPath = ga.findHotelsPath();
    std::cout << "Chemin des hôtels trouvés (" << hotelsPath.size() << " hôtels): ";
    for (int i =0; i < hotelsPath.size()-1; ++i) {
        int hotelID = hotelsPath[i];
        int nextHotelID = (i + 1 < hotelsPath.size()) ? hotelsPath[i + 1] : instance->get_Id_Hotel_Arrivee();
        std::cout <<"(" << instance->get_distance_Hotel_Hotel(hotelID, nextHotelID) << " " << instance->get_POI_Duree_Max_Voyage(i) << ")" << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << instance->get_Nombre_Jour() << std::endl;

    return 0;







    /*
    std::cout << "Début de la résolution..." << std::endl;
    int i_val_Retour_Fct_obj=0;
    Solution * uneSolution = new Solution();
    // S'assurer qu'il y a au moins un jour (évite accès hors-bornes sur v_v_Sequence_Id_Par_Jour[0])
    if (uneSolution->v_v_Sequence_Id_Par_Jour.empty()) {
        uneSolution->v_v_Sequence_Id_Par_Jour.push_back(std::vector<int>());
    }
    vector<int> v_i_tmp ;

//INITIALISATION D'UN SOLUTION EN DUR POUR L'INSTANCE 1
    v_i_tmp.clear();

    appendHotel(*uneSolution, 1);
    appendPOI(*instance, *uneSolution, 0);
    appendPOI(*instance, *uneSolution, 2);
    std::cout << "POI : ";
    for (int poi : uneSolution->v_v_Sequence_Id_Par_Jour[0]) {
        std::cout << poi << " ";
    }
    appendPOI(*instance, *uneSolution, 5,1);
    std::cout << "POI : ";
    for (int poi : uneSolution->v_v_Sequence_Id_Par_Jour[0]) {
        std::cout << poi << " ";
    }






    uneSolution->v_Id_Hotel_Intermedaire.push_back(2);
    uneSolution->v_Date_Depart.push_back(0.0);
    uneSolution->v_Date_Depart.push_back(0.0);
    v_i_tmp ={0, 2, 5, 9, 14, 21, 28, 20, 27, 35, 42, 36, 29, 22, 30, 31};
    uneSolution->v_v_Sequence_Id_Par_Jour.push_back(v_i_tmp);
    v_i_tmp ={24, 32, 40, 33, 25, 19, 26, 34, 41, 47, 52, 56, 59, 61};
    uneSolution->v_v_Sequence_Id_Par_Jour.push_back(v_i_tmp);
    uneSolution->i_valeur_fonction_objectif=816;

    
    uneSolution->Verification_Solution(instance);
    
    i_val_Retour_Fct_obj=uneSolution->i_valeur_fonction_objectif;
    delete uneSolution;
    return i_val_Retour_Fct_obj;*/
}

