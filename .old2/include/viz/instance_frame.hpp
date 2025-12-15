#pragma once
#include "instance.hpp"
#include "raylib.h"
#include "viz/frame.hpp"
#include <string>

class InstanceFrame : public Frame {
private:
  // Settings
  int world_scale;
  int hostel_radius;
  int poi_radius;
  Color hostel_color;
  Color POI_color;

  // Data
  Instance instance;
  Camera2D camera;
  int hovered_point_index = -1;

public:
  InstanceFrame(const std::string &title, const Rectangle &area,
                const Instance &instance);
  ~InstanceFrame() override;

  void update() override;
  void draw(const Color &text_color) override;
};