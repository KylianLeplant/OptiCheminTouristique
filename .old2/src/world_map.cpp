#include "world_map.hpp"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <sys/types.h>

// Returns the number of hostels in the map
int WorldMap::getHostelCount() const {
  return static_cast<int>(hostels.size());
}

// Returns the number of POIs in the map
int WorldMap::getPOICount() const { return static_cast<int>(pois.size()); }

// Returns the total number of points in the map
int WorldMap::getTotalPointCount() const {
  return getHostelCount() + getPOICount();
}

// Adds a hostel to the map
void WorldMap::addHostel(const Hostel &hostel) {
  hostels.push_back(hostel);
  // Update total point count cache.
  // This will not be a performance issue since adding hostels is done only once
  // during instance loading.
  total_point_count = getHostelCount() + getPOICount();
  // Also reserve space in the distance cache for new points
  distance_cache.assign(total_point_count * total_point_count, -1.0f);
}

// Adds a POI to the map
void WorldMap::addPOI(const POI &poi) {
  pois.push_back(poi);
  // Update total point count cache.
  // This will not be a performance issue since adding hostels is done only once
  // during instance loading.
  total_point_count = getHostelCount() + getPOICount();
  // Also reserve space in the distance cache for new points
  distance_cache.assign(total_point_count * total_point_count, -1.0f);
}

const Hostel &WorldMap::getHostelByIndex(int index) const {
  return hostels.at(index);
}

const POI &WorldMap::getPOIByIndex(int index) const { return pois.at(index); }

const Hostel &WorldMap::getStartingHostel() const {
  if (hostels.empty()) {
    throw std::runtime_error("No hostels available in the map!");
  }
  return hostels.front();
}

const Hostel &WorldMap::getEndingHostel() const {
  if (hostels.empty()) {
    throw std::runtime_error("No hostels available in the map!");
  }
  return hostels.at(1);
}

int WorldMap::getStartingHostelIndex() const { return 0; }
int WorldMap::getEndingHostelIndex() const { return 1; }

const float WorldMap::getDistanceBetweenPoints(const Point &point1,
                                               const Point &point2) const {
  // First check if both points are the same
  if (&point1 == &point2) { // Checking by adress is the fastest way to go
    return 0.0f;
  }

  // Then compute the unique key for the cache
  int id1 = point1.getGlobalID();
  int id2 = point2.getGlobalID();
  int index = id1 * total_point_count + id2;

  // Check cache : fast array lookup
  if (distance_cache[index] >= 0.0f) {
    return distance_cache[index];
  }

  // Calculate Euclidean distance
  float dx = point1.getX() - point2.getX();
  float dy = point1.getY() - point2.getY();
  float distance = sqrt(dx * dx + dy * dy);

  // Update the cache and its symmetric entry
  distance_cache[index] = distance;
  distance_cache[id2 * total_point_count + id1] = distance;

  return distance;
}