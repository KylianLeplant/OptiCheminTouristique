#include "solution.hpp"
#include "instance.hpp"
#include <iostream>
#include <set>

Solution::Solution() : score_value(0) {}
Solution::~Solution() = default;

bool Solution::isValid(const Instance &instance) {
  bool valid = true;
  std::set<int> visited_pois;
  int score = 0;

  // 1. Verifying intermediate hostels size
  if (intermediate_hostels.size() != instance.getDayCount() - 1) {
    valid = false;
    std::cout << "Error : Incorrect number of intermediate hostels.\n";
  }

  // 2. Verifying intermediate hostels IDs
  for (size_t i = 0; i < intermediate_hostels.size(); ++i) {
    if ((intermediate_hostels[i] < 0) ||
        (intermediate_hostels[i] >= instance.getWorldMap().getHostelCount())) {
      valid = false;
      std::cout << "Error : Invalid hostel ID at index " << i << ": "
                << intermediate_hostels[i] << ".\n";
    }
  }

  // 3. Verifying the poi sequence and uniqueness
  if (pois_sequence.size() != instance.getDayCount()) {
    valid = false;
    std::cout << "Error : POI sequence size does not mach day count.\n";
  }

  // 4. Verifying each POI ID and calculating score
  for (size_t day = 0; day < pois_sequence.size(); ++day) { // For each day
    for (size_t poi_id = 0; poi_id < pois_sequence[day].size();
         ++poi_id) { // For each POI
      if ((pois_sequence[day][poi_id] < 0) ||
          (pois_sequence[day][poi_id] >=
           instance.getWorldMap().getPOICount())) {
        // Invalid POI ID
        valid = false;
        std::cout << "Error : Invalid POI ID at day " << day << ", index "
                  << poi_id << ": " << pois_sequence[day][poi_id] << ".\n";
      } else {
        // Check for uniqueness and calculate score
        if (visited_pois.find(pois_sequence[day][poi_id]) ==
            visited_pois.end()) {
          visited_pois.insert(pois_sequence[day][poi_id]);
          // Add to score
          score += instance.getWorldMap()
                       .getPOIByIndex(pois_sequence[day][poi_id])
                       .getScore();
        } else {
          valid = false;
          std::cout << "Error : POI " << pois_sequence[day][poi_id]
                    << " visited multiple times.\n";
        }
      }
    }
  }

  // 5. Verifying the size of start_dates
  if (start_dates.size() != instance.getDayCount()) {
    valid = false;
    std::cout << "Error : Start dates size does not match day count.\n";
  }

  // 6. Verifying start dates validity
  for (size_t day = 0; day < start_dates.size(); ++day) { // For each day
    if ((start_dates[day] < 0) || (start_dates[day] >= 100000)) {
      valid = false;
      std::cout << "Error : Invalid start date at day " << day << ": "
                << start_dates[day] << ".\n";
    }
  }

  // 7. Verifying daily travel constraints
  for (size_t day = 0; day < instance.getDayCount(); ++day) {
    if (!pois_sequence[day].empty()) {
      float current_time = start_dates[day];
      // We are on day 'day' with POIs to visit.
      // We start and end at the appropriate hostels.
      const Hostel &starting_hostel =
          (day == 0) ? instance.getWorldMap().getStartingHostel()
                     : instance.getWorldMap().getHostelByIndex(
                           intermediate_hostels[day - 1]);
      const Hostel &ending_hostel =
          (day == instance.getDayCount() - 1)
              ? instance.getWorldMap().getEndingHostel()
              : instance.getWorldMap().getHostelByIndex(
                    intermediate_hostels[day]);

      // The first and last travel segments are special (hostel to first POI,
      // last POI to hostel).
      // Travel from a point to a POI :
      // - Check if current time + travel time is before closing time
      // - Set current time to closing time
      // For a POI-to-Hostel situation, just add travel time.

      // First segment : starting hostel to first POI
      const POI &first_poi =
          instance.getWorldMap().getPOIByIndex(pois_sequence[day][0]);
      current_time += instance.getWorldMap().getDistanceBetweenPoints(
          starting_hostel, first_poi);
      if (current_time > first_poi.getClosingTime()) {
        valid = false;
        std::cout << "Error : Arrival at POI " << first_poi.getGlobalID()
                  << " after closing time on day " << day << ".\n";
      }

      // Intermediate POI-to-POI segments
      // We need to loop until the second last POI
      for (size_t poi_idx = 0; poi_idx < pois_sequence[day].size() - 1;
           ++poi_idx) {
        const POI &current_poi =
            instance.getWorldMap().getPOIByIndex(pois_sequence[day][poi_idx]);
        const POI &next_poi = instance.getWorldMap().getPOIByIndex(
            pois_sequence[day][poi_idx + 1]);
        // Travel to next POI
        current_time += instance.getWorldMap().getDistanceBetweenPoints(
            current_poi, next_poi);
        // Check arrival time
        if (current_time > next_poi.getClosingTime()) {
          valid = false;
          std::cout << "Error : Arrival at POI " << next_poi.getGlobalID()
                    << " after closing time on day " << day << ".\n";
        }
      }

      // Last segment : last POI to ending hostel
      const POI &last_poi =
          instance.getWorldMap().getPOIByIndex(pois_sequence[day].back());
      current_time += instance.getWorldMap().getDistanceBetweenPoints(
          last_poi, ending_hostel);

      // Check daily travel duration
      float daily_travel_duration = current_time - start_dates[day];
      // TODO : add day getter to Instance
      if (daily_travel_duration > instance.getDayByIndex(day).getDuration()) {
        valid = false;
        std::cout << "Error : Exceeded max travel duration on day " << day
                  << ". Duration: " << daily_travel_duration << ".\n";
      }
    }
  }

  // Checking if the score matches
  if (score != score_value) {
    valid = false;
    std::cout << "Error : Score mismatch. Calculated: " << score
              << ", Expected: " << score_value << ".\n";
  }

  return valid;
}