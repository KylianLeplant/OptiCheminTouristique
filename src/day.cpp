#include "day.hpp"
#include <iostream>

Day::Day(float duration) : duration(duration) {}
Day::~Day() = default;

float Day::getDuration() const { return duration; }