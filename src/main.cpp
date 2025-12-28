// Could a simpler architecture benefit this project ?
// Maybe keep the Instance class as the big data holder, and keep using IDs for
// everything as intended in the initial design. This avoids having to pass
// heavy objects around.

#include "instance.hpp"
#include "solution.hpp"
#include "solver.hpp"
#include <iostream>
#include <string>

const std::string data_folder = "Data/";
const std::string data_file_list = "data.txt";
const std::string output_file_list = "sortie.txt";

int main() {
  Instance inst(data_folder + "Inst1.txt");

  Solver solv(inst, 42);

  Solution best_sol = solv.solve(120,   // Time Limit (seconds)
                                 4000,  // Population Size (Huge!)
                                 0.02f, // Elitism Rate (Keep top 2%)
                                 4,     // Tournament Size (Gentle pressure)
                                 0.45f  // Mutation Rate (Very High)
  );
  // Solution sol;

  // sol.intermediate_hostels.push_back(2);
  // sol.start_dates.push_back(0.0f);
  // sol.start_dates.push_back(0.0f);
  // sol.pois_sequence.push_back(
  //     {0, 2, 5, 9, 14, 21, 28, 20, 27, 35, 42, 36, 29, 22, 30, 31});
  // sol.pois_sequence.push_back(
  //     {24, 32, 40, 33, 25, 19, 26, 34, 41, 47, 52, 56, 59, 61});
  // sol.score_value = 816;

  bool valid = best_sol.isValid(inst);
  if (valid) {
    std::cout << "Solution is valid.\n";
    std::cout << "Total Score: " << best_sol.score_value << "\n";
    // Display solution details
    for (size_t day = 0; day < best_sol.pois_sequence.size(); ++day) {
      std::cout << "Day " << day + 1 << ":\n";
      std::cout << "  Start Date: " << best_sol.start_dates[day] << "\n";
      std::cout << "  Intermediate Hostel: "
                << best_sol.intermediate_hostels[day] << "\n";
      std::cout << "  POIs Visited: ";
      for (const auto &poi_id : best_sol.pois_sequence[day]) {
        std::cout << poi_id << " ";
      }
      std::cout << "\n";
    }
  } else {
    std::cout << "Solution is invalid.\n";
  }

  return 0;
}