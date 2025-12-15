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

  // 1. Open the file
  std::ifstream input_file(file_path);
  if (!input_file.is_open()) {
    std::cerr << "Error opening the file: " << file_path << std::endl;
    throw std::runtime_error("CRITICAL ERROR: Could not open the file!");
  }

  int line_number = 0;
  std::string line; // Current line being read
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

    // Now 'rowData' contains all the tokens in the current line.
    // We can process it according to the line number.
    switch (line_number) {
    case 1:  // First line
      break; // Nothing useful in this line : we will deduce counts later
    case 2:  // Second line
             // TODO : max_exploration_hours = std::stof(rowData[0]); if needed.
      break;
    case 3: // Third line : load day durations
      for (const std::string &duration_str : rowData) {
        days.push_back(std::stof(duration_str));
      }
      break;
    case 4: // Fourth line : blank, skip
      break;
    default: // Fifth line and onwards : load points
      if (rowData.size() < 7) {
        std::cerr << "Error: Malformed line " << line_number << " in file "
                  << file_path << std::endl;
        throw std::runtime_error("CRITICAL ERROR: Malformed data line!");
      }

      std::string point_label = rowData[0];
      float x = std::stof(rowData[1]);
      float y = std::stof(rowData[2]);
      float score = std::stof(rowData[3]);
      // float always_zero = std::stof(rowData[4]); // Unused
      float opening_time = std::stof(rowData[5]);
      float closing_time = std::stof(rowData[6]);

      // Check if it's a Hostel or a POI
      if (point_label[0] == 'H') {
        // It's a Hostel
        Hostel hostel(x, y, point_label);
        hostels.push_back(hostel);
      } else if (point_label[0] == 'C') {
        // It's a POI
        POI poi(x, y, point_label, score, opening_time, closing_time);
        pois.push_back(poi);
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
  std::cout << "Loaded " << getHostelCount() << " hostels.\n"
            << "Loaded " << getPOICount() << " POIs.\n"
            << "Loaded " << days.size() << " days.\n";

  input_file.close(); // Close the file when done reading

  // Load distance cache
  total_points = getPOICount() + getHostelCount();
  distance_cache.assign(total_points * total_points, -1.0f);
}

int Instance::getHostelCount() const {
  return static_cast<int>(hostels.size());
}
int Instance::getPOICount() const { return static_cast<int>(pois.size()); }
int Instance::getTotalPointCount() const { return total_points; }

int Instance::getDayCount() const { return static_cast<int>(days.size()); }
float Instance::getDayDuration(int day_index) const {
  return days.at(day_index);
}

int Instance::getStartingHostelID() const { return 0; }
int Instance::getEndingHostelID() const { return 1; }

const Hostel &Instance::getHostelById(int id) const { return hostels.at(id); }
const POI &Instance::getPOIById(int id) const { return pois.at(id); }

const float Instance::getDistancePOIPOI(int id1, int id2) const {
  const int index1 = getHostelCount() + id1;
  const int index2 = getHostelCount() + id2;
  const int cache_index = index1 * total_points + index2;

  // Check if distance is already cached
  if (distance_cache[cache_index] >= 0.0f) {
    return distance_cache[cache_index];
  }

  const POI &poi1 = getPOIById(id1);
  const POI &poi2 = getPOIById(id2);

  float dx = poi1.getX() - poi2.getX();
  float dy = poi1.getY() - poi2.getY();
  float distance = std::sqrt(dx * dx + dy * dy);

  // Cache the computed distance
  distance_cache[cache_index] = distance;
  distance_cache[index2 * total_points + index1] = distance; // Symmetric

  return distance;
}

const float Instance::getDistanceHostelHostel(int id1, int id2) const {
  const int cache_index = id1 * total_points + id2;

  // Check if distance is already cached
  if (distance_cache[cache_index] >= 0.0f) {
    return distance_cache[cache_index];
  }

  const Hostel &hostel1 = getHostelById(id1);
  const Hostel &hostel2 = getHostelById(id2);

  float dx = hostel1.getX() - hostel2.getX();
  float dy = hostel1.getY() - hostel2.getY();
  float distance = std::sqrt(dx * dx + dy * dy);

  // Cache the computed distance
  distance_cache[cache_index] = distance;
  distance_cache[id2 * total_points + id1] = distance; // Symmetric

  return distance;
}

const float Instance::getDistanceHostelPOI(int hostel_id, int poi_id) const {
  const int index1 = hostel_id;
  const int index2 = getHostelCount() + poi_id;
  const int cache_index = index1 * total_points + index2;

  // Check if distance is already cached
  if (distance_cache[cache_index] >= 0.0f) {
    std::cout << distance_cache[cache_index] << "\n";
  }

  const Hostel &hostel = getHostelById(hostel_id);
  const POI &poi = getPOIById(poi_id);

  float dx = hostel.getX() - poi.getX();
  float dy = hostel.getY() - poi.getY();
  float distance = std::sqrt(dx * dx + dy * dy);

  // Cache the computed distance
  distance_cache[cache_index] = distance;
  distance_cache[index2 * total_points + index1] = distance; // Symmetric

  return distance;
}

const float Instance::getPOIOpeningTime(int poi_id) const {
  return getPOIById(poi_id).getOpeningTime();
}

const float Instance::getPOIClosingTime(int poi_id) const {
  return getPOIById(poi_id).getClosingTime();
}

const float Instance::getPOIVisitDuration(int poi_id) const {
  return getPOIById(poi_id).getVisitDuration();
}

const float Instance::getPOIScore(int poi_id) const {
  return getPOIById(poi_id).getScore();
}

const float Instance::getHostelX(int hostel_id) const {
  return getHostelById(hostel_id).getX();
}

const float Instance::getHostelY(int hostel_id) const {
  return getHostelById(hostel_id).getY();
}

const float Instance::getPOIX(int poi_id) const {
  return getPOIById(poi_id).getX();
}

const float Instance::getPOIY(int poi_id) const {
  return getPOIById(poi_id).getY();
}