#include "hostel.hpp"
#include "point.hpp"

// === LIFECYCLE ===

Hostel::Hostel() : Point() {}
Hostel::Hostel(float x, float y, const std::string &label)
    : Point(x, y, label) {}

Hostel::~Hostel() = default;