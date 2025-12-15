#pragma once
#include "raylib.h"
#include "viz/frame.hpp"
#include <memory> // Required for std::unique_ptr
#include <vector>

class Visualizer {
private:
  // Settings
  Color background_color;
  Color header_color;
  Color text_color;
  int header_height; // In pixels
  int starting_width;
  int starting_height;

  // Logic
  std::vector<std::unique_ptr<Frame>> frames;
  int active_frame_index;

  // Elements
  Rectangle prev_button;
  Rectangle next_button;

  void drawHeader();

public:
  Visualizer(int width, int height);
  ~Visualizer();

  void run();
  void addFrame(std::unique_ptr<Frame> frame);
  Rectangle getDefaultArea();
};