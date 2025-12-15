// #define CHEMIN_DOSSIER_DONNEES "Data/"
// #define NOM_FICHIER_LISTE_FICHIER_DONNEES "data.txt"
// #define NOM_FICHIER_LISTE_SORTIE "sortie.txt"

#include "genetic_algorithm.hpp"
#include "instance.hpp"
#include "solution.hpp"
#include "viz/frame.hpp"
#include "viz/instance_frame.hpp"
#include "viz/visualizer.hpp"
#include <iostream>
#include <string>

const std::string data_folder = "Data/";
const std::string data_file_list = "data.txt";
const std::string output_file_list = "sortie.txt";

int main() {

  Instance inst2(data_folder + "Inst1.txt");
  Instance inst(data_folder + "Inst4.txt");

  GeneticAlgorithm ga(inst2);
  std::vector<int> path = ga.findHostelsPath();
  std::cout << "Computing hostels path:\n";
  for (int hostel : path) {
    std::cout << hostel << " - "
              << inst2.getWorldMap().getHostelByIndex(hostel).getLabel()
              << "\n";
  }

  // Hardcoded solution for testing validation
  Solution sol(inst2);

  sol.intermediate_hostels.push_back(2);
  sol.start_dates.push_back(0.0f);
  sol.start_dates.push_back(0.0f);
  sol.pois_sequence.push_back(
      {0, 2, 5, 9, 14, 21, 28, 20, 27, 35, 42, 36, 29, 22, 30, 31});
  sol.pois_sequence.push_back(
      {24, 32, 40, 33, 25, 19, 26, 34, 41, 47, 52, 56, 59, 61});
  sol.score_value = 816;

  bool valid = sol.isValid();
  if (valid) {
    std::cout << "Solution is valid.\n";
  } else {
    std::cout << "Solution is invalid.\n";
  }

  // Visualizer viz(800, 600);
  ////viz.addFrame(std::make_unique<InstanceFrame>("Instance Frame",
  ////                                             viz.getDefaultArea(), inst));
  // viz.addFrame(std::make_unique<InstanceFrame>("Instance Frame 2",
  //                                              viz.getDefaultArea(), inst2));
  // viz.addFrame(std::make_unique<Frame>("Basic Frame", viz.getDefaultArea()));
  // viz.addFrame(
  //     std::make_unique<Frame>("Another Basic Frame", viz.getDefaultArea()));
  // viz.run();

  return 0;
}
