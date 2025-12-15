#pragma once

class Day {
private:
  float duration;

public:
  Day(float duration);
  ~Day();

  float getDuration() const;
};