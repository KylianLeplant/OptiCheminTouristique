#include "instance.hpp"

#include <fstream>   // Required for std::ifstream
#include <iostream>  // Required for std::cerr, std::endl
#include <sstream>   // Required for std::stringstream
#include <stdexcept> // Required for std::runtime_error
#include <string>    // Required for std::string
#include <vector>    // Required for std::vector

Instance::Instance(const std::string &file_path) : file_path(file_path) {
  // First line :
  // - number of POIs + 1
  // - number of Hostels - 2
  // - number of days
  //
  // Second line :
  // - maximum exploration hours, i.e. sum of all day durations
  //
  // Third line :
  // - day durations (one float per day)
  //
  // Fourth line is blank.
  //
  // Fifth line and onwards :
  // - HX or CX : Hostel or POI and its ID. Only used as a label here.
  // - x coordinate (float)
  // - y coordinate (float)
  // - POI score. Zero for Hostels.
  // - Always zero.
  // - Opening time. Zero for Hostels.
  // - Closing time. Equals maximum exploration hours for Hostels.

  // First, open the file
  std::ifstream input_file(file_path);
  if (!input_file.is_open()) {
    std::cerr << "Error opening the file: " << file_path << std::endl;
    throw std::runtime_error("CRITICAL ERROR: Could not open the file!");
  }

  int line_number = 0;
  std::string line; // current line being read
  while (std::getline(input_file, line)) {
    line_number++;
    // 'line' now contains the text of the current row
    // Next step : parse the line according to the expected format.
    // This is best achieved by first creating a vector of tokens split by
    // spaces.
    std::stringstream ss(line); // Wrap the line in a stringstream
    std::string segment;
    std::vector<std::string> rowData;
    while (ss >> segment) { // Extract space-separated tokens
      rowData.push_back(segment);
    }

    // Now 'rowData' contains all tokens in the current line.
    // We can process it according to the line number.
    switch (line_number) {
    case 1:  // First line
      break; // Nothing useful in this line : we will deduce counts later
    case 2:  // Second line
      max_exploration_hours = std::stof(rowData[0]);
      break;
    case 3: // Third line : load day durations
      for (const std::string &duration_str : rowData) {
        float duration = std::stof(duration_str);
        Day d(duration);
        days.emplace_back(d);
      }
      break;
    case 4:  // Fourth line (blank)
      break; // NOOP
    default: // Fifth line and onwards
      if (rowData.size() < 7) {
        std::cerr << "Error: Malformed data line at line " << line_number
                  << std::endl;
        throw std::runtime_error("CRITICAL ERROR: Malformed data line!");
      }

      std::string point_label = rowData[0];
      float x = std::stof(rowData[1]);
      float y = std::stof(rowData[2]);
      float score = std::stof(rowData[3]);
      float opening_time = std::stof(rowData[5]);
      float closing_time = std::stof(rowData[6]);

      // Check wether the point is a Hostel or a POI.
      if (point_label[0] == 'H') {
        Hostel h(x, y, point_label);
        world_map.addHostel(h);
      } else if (point_label[0] == 'C') {
        POI p(x, y, point_label, score, opening_time, closing_time);
        world_map.addPOI(p);
      } else {
        std::cerr << "Error: Unknown point type at line " << line_number
                  << std::endl;
        throw std::runtime_error("CRITICAL ERROR: Unknown point type!");
      }
      break;
    }
  }

  std::cout << "Finished reading file: " << file_path << std::endl;
  // Display the amount of data loaded
  std::cout << "Loaded " << world_map.getHostelCount() << " hostels."
            << std::endl;
  std::cout << "Loaded " << world_map.getPOICount() << " POIs." << std::endl;
  std::cout << "Loaded " << days.size() << " days." << std::endl;

  input_file.close(); // Close the file when done reading
}

const WorldMap &Instance::getWorldMap() const { return world_map; }

int Instance::getDayCount() const { return static_cast<int>(days.size()); }

const Day &Instance::getDayByIndex(const int index) const {
  return days.at(index);
}