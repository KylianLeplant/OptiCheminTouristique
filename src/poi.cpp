#include "poi.hpp"
#include "point.hpp"

// === LIFECYCLE ===

// Default constructor
POI::POI() : Point(), score(0.0f), opening_time(0.0f), closing_time(0.0f) {};
POI::POI(float x, float y, const std::string &label, float score,
         float opening_time, float closing_time)
    : Point(x, y, label), score(score), opening_time(opening_time),
      closing_time(closing_time) {}

// Default destructor
POI::~POI() = default;

// === GETTERS / SETTERS

float POI::getScore() const { return score; }
float POI::getVisitDuration() const { return closing_time - opening_time; }
float POI::getOpeningTime() const { return opening_time; }
float POI::getClosingTime() const { return closing_time; }