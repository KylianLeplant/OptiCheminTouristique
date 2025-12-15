#include "world_map.hpp"
#include <stdexcept>

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
void WorldMap::addHostel(const Hostel &hostel) { hostels.push_back(hostel); }

// Adds a POI to the map
void WorldMap::addPOI(const POI &poi) { pois.push_back(poi); }

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