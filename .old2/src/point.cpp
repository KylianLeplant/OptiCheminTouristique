#include "point.hpp"
#include "utils.hpp"
#include <sys/types.h>

// Initialize static member
int Point::next_id = 0;

// === LIFECYCLE ===

// Default constructor
Point::Point() : global_id(next_id++), x(0.0f), y(0.0f), label("Unknown") {}

// Utility constructor
Point::Point(float x, float y, const std::string &label)
    : global_id(next_id++), x(x), y(y), label(label) {}
// Virtual destructor
Point::~Point() = default;

// === GETTERS / SETTERS ===

// Returns the point's x coordinate
float Point::getX() const { return x; }

// Returns the point's y coordinate
float Point::getY() const { return y; }

// Returns the point's label
std::string Point::getLabel() const { return label; }

// Returns the point's ID
int Point::getGlobalID() const { return global_id; }