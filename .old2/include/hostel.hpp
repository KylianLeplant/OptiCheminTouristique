#pragma once

#include "point.hpp"
#include <string>

// Represents an Hostel that acts as a starting and ending destination for a
// day.
// This class is functionally identical to a Point, but is defined separately
// for clarity.
class Hostel : public Point {
private:
public:
  Hostel();
  Hostel(float x, float y, const std::string &label);

  ~Hostel() override;
};