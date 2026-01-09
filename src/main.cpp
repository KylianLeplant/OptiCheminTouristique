#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "instance.hpp"
#include "solution.hpp"
#include "solver.hpp"

#define CHEMIN_DOSSIER_DONNEES "Data/"
#define NOM_FICHIER_LISTE_FICHIER_DONNEES "data.txt"
#define NOM_FICHIER_LISTE_SORTIE "sortie.txt"

int main() {

  std::cout << "=== DEBUT DU PROGRAMME ===" << std::endl;
  std::cout << std::flush;

  std::cout << "Avant ouverture fichiers" << std::endl;
  std::cout << std::flush;

  std::string s_chemin_donnees = CHEMIN_DOSSIER_DONNEES;
  std::string s_chemin_liste =
      s_chemin_donnees + NOM_FICHIER_LISTE_FICHIER_DONNEES;

  std::cout << "Chemin: " << s_chemin_liste << std::endl;
  std::cout << std::flush;

  std::ifstream fichier_Liste(s_chemin_liste.c_str(), std::ios::in);

  // Output file
  std::string s_chemin_sortie = s_chemin_donnees + NOM_FICHIER_LISTE_SORTIE;

  // Also clears existing content on launch
  std::ofstream fichier_Sortie(s_chemin_sortie.c_str());

  if (fichier_Liste) {
    if (fichier_Sortie) {
      fichier_Sortie << " Fichier données\t Tps de résolution \t Best solution"
                     << std::endl;

      std::string s_nom_instance;
      while (getline(fichier_Liste, s_nom_instance)) {

        // --- SANITIZATION ---
        // Remove \r and \n to handle Windows/Linux line ending differences
        s_nom_instance.erase(
            std::remove(s_nom_instance.begin(), s_nom_instance.end(), '\r'),
            s_nom_instance.end());
        s_nom_instance.erase(
            std::remove(s_nom_instance.begin(), s_nom_instance.end(), '\n'),
            s_nom_instance.end());

        if (s_nom_instance.empty())
          continue;

        std::string s_chemin_instance = s_chemin_donnees + s_nom_instance;

        // Start Resolution
        std::cout << " Résolution de " << s_nom_instance << std::endl;

        try {
          // Load Instance
          Instance instance(s_chemin_instance);

          // Initialize Solver (Seed 65)
          Solver solv(instance, 65);

          // Start Timer
          auto chrono_start = std::chrono::system_clock::now();

          // Run Genetic Algorithm
          Solution best_sol =
              solv.solve(59,    // Time Limit (seconds)
                         1000,  // Population Size
                         0.02f, // Elitism Rate
                         10,    // Tournament Size
                         0.3f,  // Mutation Rate
                         0.01f, // Intensify Rate
                         100,   // Intensify Iterations
                         0.01f  // Intensify Generation Probability
              );

          // Stop Timer
          auto chrono_end = std::chrono::system_clock::now();
          std::chrono::duration<double> elapsed = chrono_end - chrono_start;

          // End Resolution
          std::cout << " Fin de résolution de " << s_nom_instance << std::endl;

          // Write to file if valid
          if (best_sol.isValid(instance)) {
            fichier_Sortie << s_chemin_instance << "\t" << elapsed.count()
                           << "\t" << best_sol.score_value << std::endl;
          } else {
            fichier_Sortie << s_chemin_instance << "\t" << elapsed.count()
                           << "\t"
                           << "INVALID" << std::endl;
          }

        } catch (const std::exception &e) {
          std::cerr << "Erreur exception : " << e.what() << std::endl;
        }
      }
      fichier_Sortie.close();
    } else {
      std::cout
          << " Erreur lecture des données : chemin vers la sortie non valide. "
          << std::endl;
    }
    fichier_Liste.close();
  } else {
    std::cout << " Erreur lecture des données : chemin listant l'ensemble des "
                 "données non valide. "
              << std::endl;
  }

  return 0;
}