// #define CHEMIN_DOSSIER_DONNEES "Data/"
// #define NOM_FICHIER_LISTE_FICHIER_DONNEES "data.txt"
// #define NOM_FICHIER_LISTE_SORTIE "sortie.txt"

#include "instance.hpp"
#include "viz/frame.hpp"
#include "viz/instance_frame.hpp"
#include "viz/visualizer.hpp"
#include <iostream>
#include <string>

const std::string data_folder = "Data/";
const std::string data_file_list = "data.txt";
const std::string output_file_list = "sortie.txt";

int main() {

  // Hostel h(10.0f, 20.0f, "Unknown Point");
  // std::cout << h.getX() << "\n";
  // std::cout << h.getY() << "\n";
  // std::cout << h.getLabel() << "\n";
  // std::cout << h.getID() << "\n";

  Instance inst(data_folder + "Inst20.txt");
  Instance inst2(data_folder + "Inst1.txt");
  // std::cout << inst.getWorldMap().getTotalPointCount() << "\n";
  // Hostel h = inst.getWorldMap().getEndingHostel();
  // std::cout << h.getLabel() << "\n";

  // std::cout << inst.getDayCount() << "\n";

  Visualizer viz(800, 600);
  viz.addFrame(std::make_unique<InstanceFrame>("Instance Frame",
                                               viz.getDefaultArea(), inst));
  viz.addFrame(std::make_unique<InstanceFrame>("Instance Frame 2",
                                               viz.getDefaultArea(), inst2));
  viz.addFrame(std::make_unique<Frame>("Basic Frame", viz.getDefaultArea()));
  viz.addFrame(
      std::make_unique<Frame>("Another Basic Frame", viz.getDefaultArea()));
  viz.run();

  return 0;
}
