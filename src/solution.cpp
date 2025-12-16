#include "solution.hpp"
#include "instance.hpp"

#include <iostream> // Required for std::cout
#include <set>

Solution::Solution() : score_value(0) {}
Solution::~Solution() = default;

bool Solution::isValid(const Instance &inst) const {

  // CHECK LIST
  // 1. The number of intermediate hostels matches the number of days - 1
  // 2. All intermediate hostel IDs are valid (between 0 and hostel count - 1)
  // 3. The number of POI sequences matches the number of days
  // 4. All POI IDs are valid (between 0 and poi count - 1)
  // 5. No POI is visited more than once
  // 6. Arrival times at each POI are within opening hours
  // 7. The number of days matches the instance's numbers of days.
  // 8. The duration of each day is within the allowed duration
  // 9. The calculated score matches the stored score_value

  std::cout << "Starting solution validation...\n";

  bool valid = true;

  // 1. The number of intermediate hostels matches the number of days - 1
  if (intermediate_hostels.size() != inst.getDayCount() - 1) {
    valid = false;
    std::cout << "Error : Incorrect number of intermediate hostels.\n";
  }
  if (valid) {
    std::cout << "Passed intermediate hostels count check.\n";
  }

  // 2. All intermediate hostel IDs are valid (between 0 and hostel count - 1)
  for (int hostel_index = 0; hostel_index < intermediate_hostels.size();
       ++hostel_index) {
    if ((intermediate_hostels[hostel_index] < 0) ||
        (intermediate_hostels[hostel_index] >= inst.getHostelCount())) {
      valid = false;
      std::cout << "Error : Invalid hostel ID at index " << hostel_index << ": "
                << intermediate_hostels[hostel_index] << ".\n";
    }
  }
  if (valid) {
    std::cout << "Passed intermediate hostels ID validity check.\n";
  }

  // 3. The number of POI sequences matches the number of days
  if (pois_sequence.size() != inst.getDayCount()) {
    valid = false;
    std::cout << "Error : POI sequence size does not match day count.\n";
  }
  if (valid) {
    std::cout << "Passed POI sequence count check.\n";
  }

  // 4. All POI IDs are valid (between 0 and poi count - 1)
  for (int day = 0; day < pois_sequence.size(); ++day) {
    for (int poi_index = 0; poi_index < pois_sequence[day].size();
         ++poi_index) {
      if ((pois_sequence[day][poi_index] < 0) ||
          (pois_sequence[day][poi_index] >= inst.getPOICount())) {
        valid = false;
        std::cout << "Error : Invalid POI ID at day " << day << ", index "
                  << poi_index << ": " << pois_sequence[day][poi_index]
                  << ".\n";
      }
    }
  }
  if (valid) {
    std::cout << "Passed POI ID validity check.\n";
  }

  // 5. No POI is visited more than once
  std::set<int> visited_pois;
  for (int day = 0; day < pois_sequence.size(); ++day) {
    for (int poi_index = 0; poi_index < pois_sequence[day].size();
         ++poi_index) {
      int poi_id = pois_sequence[day][poi_index];
      if (visited_pois.find(poi_id) == visited_pois.end()) {
        visited_pois.insert(poi_id);
      } else {
        valid = false;
        std::cout << "Error : POI " << poi_id << " visited multiple times.\n";
      }
    }
  }
  if (valid) {
    std::cout << "Passed POI uniqueness check.\n";
  }

  // 6. Arrival times at each POI are within opening hours
  for (int day = 0; day < pois_sequence.size(); ++day) {
    if (!pois_sequence[day].empty()) { // No POI can still be a valid solution
      float current_time = start_dates[day];
      // We are on day 'day' with POIs to visit.
      // We start and end at the appropriate hostels.
      int starting_hostel = (day == 0) ? inst.getStartingHostelID()
                                       : intermediate_hostels[day - 1];
      int ending_hostel = (day == inst.getDayCount() - 1)
                              ? inst.getEndingHostelID()
                              : intermediate_hostels[day];

      // First segment : starting hostel to first POI
      int first_poi = pois_sequence[day][0];

      current_time += inst.getDistanceHostelPOI(starting_hostel, first_poi);

      // Check arrival time
      if (current_time > inst.getPOIClosingTime(first_poi)) {
        valid = false;
        std::cout << "Error : Arrival at POI " << first_poi
                  << " after closing time on day " << day << ".\n";
      }

      // current_time must be set to opening time if we arrive earlier
      current_time = std::max(current_time, inst.getPOIOpeningTime(first_poi));

      // Intermediate POI-to-POI segments
      // We need to loop until the second last POI
      for (size_t poi_idx = 0; poi_idx < pois_sequence[day].size() - 1;
           ++poi_idx) {
        int current_poi = pois_sequence[day][poi_idx];
        int next_poi = pois_sequence[day][poi_idx + 1];
        // Travel to next POI
        current_time += inst.getDistancePOIPOI(current_poi, next_poi);
        // Check arrival time
        if (current_time > inst.getPOIClosingTime(next_poi)) {
          valid = false;
          std::cout << "Error : Arrival at POI " << next_poi
                    << " after closing time on day " << day << ".\n";
        }

        // current_time must be set to opening time if we arrive earlier
        current_time = std::max(current_time, inst.getPOIOpeningTime(next_poi));
      }

      // Last segment : last POI to ending hostel
      int last_poi = pois_sequence[day].back();
      current_time += inst.getDistanceHostelPOI(ending_hostel, last_poi);

      // Check daily travel duration
      float daily_travel_duration = current_time - start_dates[day];
      if (daily_travel_duration > inst.getDayDuration(day)) {
        valid = false;
        std::cout << "Error : Daily travel duration exceeded on day " << day
                  << ".\n";
      }
    }
  }
  if (valid) {
    std::cout << "Passed POI arrival time check.\n";
  }

  // 7. The number of days matches the instance's numbers of days.
  if (start_dates.size() != inst.getDayCount()) {
    valid = false;
    std::cout << "Error : Start dates size does not match day count.\n";
  }
  if (valid) {
    std::cout << "Passed start dates count check.\n";
  }

  // 8. The duration of each day is within the allowed duration
  for (int day = 0; day < start_dates.size(); ++day) {
    if (start_dates[day] < 0.0f ||
        start_dates[day] > inst.getDayDuration(day)) {
      valid = false;
      std::cout << "Error : Invalid start date for day " << day << ".\n";
    }
  }
  if (valid) {
    std::cout << "Passed start dates validity check.\n";
  }

  // 9. The calculated score matches the stored score_value
  int calculated_score = 0;
  for (int day = 0; day < pois_sequence.size(); ++day) {
    for (int poi_index = 0; poi_index < pois_sequence[day].size();
         ++poi_index) {
      int poi_id = pois_sequence[day][poi_index];
      calculated_score += inst.getPOIScore(poi_id);
    }
  }
  if (calculated_score != score_value) {
    valid = false;
    std::cout << "Error : Score mismatch. Calculated: " << calculated_score
              << ", Expected: " << score_value << ".\n";
  }
  if (valid) {
    std::cout << "Passed score validity check.\n";
  }

  return valid;
}