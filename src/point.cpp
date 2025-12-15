#include "point.hpp"
#include "utils.hpp"

// === LIFECYCLE ===

// Default constructor
Point::Point()
    : id(utils::GenerateRandomID()), x(0.0f), y(0.0f), label("Unknown") {}

// Utility constructor
Point::Point(float x, float y, const std::string &label)
    : x(x), y(y), id(utils::GenerateRandomID()), label(label) {}

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
std::string Point::getID() const { return id; }