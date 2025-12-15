#pragma once

#include <stdio.h>
#include <string>

// Represents a point in 2D space
class Point {
private:
  std::string id;    // This Point's ID, generated randomly
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
  std::string getID() const;    // Returns the point's ID
};

// Compute the distance between two points on the map.
// TODO : implement this the modern cpp way, as a method with const &other
// float Distance(const Point &point1, const Point &point2);