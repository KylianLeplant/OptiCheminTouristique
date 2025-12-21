#pragma once

#include "hostel.hpp"
#include "poi.hpp"
#include <cstdint>
#include <map>
#include <unordered_map>
#include <vector>

// Represents a 2D map that contains Hostels and POIs.
// This is the data structure that actually owns the Hostel and POI data.
// This class does not store any information about the problem instance,
// only the geographical data.
class WorldMap {
private:
  std::vector<Hostel> hostels; // A vector that contains the map's hostels
  std::vector<POI> pois;       // A vector that contains the map's POIs

  mutable std::vector<float> distance_cache;
  mutable int total_point_count = -1; // Cached total point count

public:
  WorldMap() = default;
  ~WorldMap() = default;

  int getHostelCount() const; // Returns the number of hostels in the map
  int getPOICount() const;    // Returns the number of POIs in the map
  int getTotalPointCount()
      const; // Returns the total number of points in the map

  // Adds a hostel to the map
  // Copies the hostel data into the map's internal storage
  // This works because Hostel is an aggregation of std types.
  void addHostel(const Hostel &hostel);

  // Adds a POI to the map
  // Copies the POI data into the map's internal storage
  // This works because POI is an aggregation of std types.
  void addPOI(const POI &poi);

  const Hostel &getHostelByIndex(
      int index) const; // Returns a copy of the hostel at the given index
  const POI &getPOIByIndex(
      int index) const; // Returns a copy of the POI at the given index

  // Returns a copy of the starting hostel
  const Hostel &getStartingHostel() const;

  // Returns a copy of the ending hostel
  const Hostel &getEndingHostel() const;

  // Returns the index of the starting hostel (always 0)
  int getStartingHostelIndex() const;

  // Returns the index of the ending hostel (always 1)
  int getEndingHostelIndex() const;

  // Returns the distance between two points on the map.
  // Must handle both hostels and POIs.
  // Must handle caching. (TODO)
  const float getDistanceBetweenPoints(const Point &point1,
                                       const Point &point2) const;

  std::vector<int> getPOIsIDs() const;
};