#pragma once
#include "hostel.hpp"
#include "poi.hpp"
#include "point.hpp"
#include <string>
#include <vector>

class Instance {
private:
  // The file this instance was loaded from
  std::string file_path;

  // All hostels in the map. Position in the vector is their ID.
  std::vector<Hostel> hostels;

  // All POIs in the map. Position in the vector is their ID.
  std::vector<POI> pois;

  // Cache for computing distances between points
  mutable std::vector<float> distance_cache;

  int total_points; // Total number of points (hostels + POIs) cached for
                    // performance.

  // Duration of each day in the trip
  std::vector<float> days;

public:
  Instance(const std::string &file_path);
  ~Instance() = default;

  int getHostelCount() const;
  int getPOICount() const;
  int getTotalPointCount() const;

  int getDayCount() const;
  float getDayDuration(int day_index) const;

  int getStartingHostelID() const;
  int getEndingHostelID() const;

  const Hostel &getHostelById(int id) const;
  const POI &getPOIById(int id) const;

  const float getDistancePOIPOI(int id1, int id2) const;
  const float getDistanceHostelHostel(int id1, int id2) const;
  const float getDistanceHostelPOI(int hostel_id, int poi_id) const;

  const float getPOIOpeningTime(int poi_id) const;
  const float getPOIClosingTime(int poi_id) const;
  const float getPOIVisitDuration(int poi_id) const;
  const float getPOIScore(int poi_id) const;

  const float getHostelX(int hostel_id) const;
  const float getHostelY(int hostel_id) const;
  const float getPOIX(int poi_id) const;
  const float getPOIY(int poi_id) const;
};