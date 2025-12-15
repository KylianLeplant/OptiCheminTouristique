#pragma once

#include <stdio.h>
#include <string>
#include <sys/types.h>

// Represents a point in 2D space
class Point {

protected:
  static int next_id;

private:
  int global_id;     // Global unique integer ID used internally
  std::string label; // The label/name of this point, loaded from the data file.
  float x;           // x coordinate
  float y;           // y coordinate

public:
  Point();                                           // Default constructor
  Point(float x, float y, const std::string &label); // Utility constructor
  virtual ~Point();                                  // Virtual destructor

  float getX() const;           // Returns the point's x coordinate
  float getY() const;           // Returns the point's y coordinate
  std::string getLabel() const; // Returns the point's label
  int getGlobalID() const;      // Returns the point's ID
};

// Compute the distance between two points on the map.
// TODO : implement this the modern cpp way, as a method with const &other
// float Distance(const Point &point1, const Point &point2);