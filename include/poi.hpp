#pragma once

#include "point.hpp"
#include <string>

// Represents a POI that can be visited to earn points
class POI : public Point {
private:
  float score;        // The score this POI gives when visited
  float opening_time; // Opening time for this POI
  float closing_time; // Closing time for this POI
public:
  POI(); // Default constructor
  POI(float x, float y, const std::string &label, float score,
      float opening_time, float closing_time); // Utility constructor
  ~POI() override;                             // Destructor

  float getScore() const;         // Returns the score of this POI
  float getVisitDuration() const; // Returns the visit duration of this POI
  float getOpeningTime() const;   // Returns the opening time of this POI
  float getClosingTime() const;   // Returns the closing time of this POI
};