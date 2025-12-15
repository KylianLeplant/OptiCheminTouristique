#include "viz/visualizer.hpp"
#include "raylib.h"
#include <iostream>

Visualizer::Visualizer(int width, int height) {
  background_color = {49, 52, 69, 255}; // Light gray background
  header_color = {36, 38, 51, 255};     // Darker gray header
  text_color = {200, 208, 242, 255};    // Blueish text color
  header_height = 30;                   // In pixels
  starting_width = width;
  starting_height = height;

  active_frame_index = 0;

  // Initialize buttons
  // First, get the size by having a 5px padding from each side to the header
  // border
  int button_size = header_height - 10;
  // The buttons are squares and right-aligned in the header
  prev_button = Rectangle{
      static_cast<float>(starting_width - 10 - 2 * button_size), 5.0f,
      static_cast<float>(button_size), static_cast<float>(button_size)};
  next_button = Rectangle{static_cast<float>(starting_width - 5 - button_size),
                          5.0f, static_cast<float>(button_size),
                          static_cast<float>(button_size)};
};
Visualizer::~Visualizer() = default;

void Visualizer::drawHeader() {

  // Header background
  DrawRectangle(0, 0, GetScreenWidth(), header_height, header_color);

  std::string header_info = "[" + std::to_string(active_frame_index + 1) + "/" +
                            std::to_string(static_cast<int>(frames.size())) +
                            "] " + frames.at(active_frame_index)->getTitle();
  // Header text
  DrawText(header_info.c_str(), 10, 7, 20, text_color);

  // Header buttons
  DrawRectangleRec(prev_button, background_color);
  DrawText("<", static_cast<int>(prev_button.x) + 10,
           static_cast<int>(prev_button.y) + 5, 20, text_color);
  DrawRectangleRec(next_button, background_color);
  DrawText(">", static_cast<int>(next_button.x) + 10,
           static_cast<int>(next_button.y) + 5, 20, text_color);

  // Hovered button effect : draw an outline
  Vector2 mouse_pos = GetMousePosition();
  if (CheckCollisionPointRec(mouse_pos, prev_button)) {
    DrawRectangleLinesEx(prev_button, 2.0f, text_color);
    // Set cursor to hand
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      // Go to previous frame
      active_frame_index--;
      if (active_frame_index < 0) {
        active_frame_index = static_cast<int>(frames.size()) - 1;
      }
    }
  }

  if (CheckCollisionPointRec(mouse_pos, next_button)) {
    DrawRectangleLinesEx(next_button, 2.0f, text_color);
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      // Go to next frame
      active_frame_index++;
      if (active_frame_index >= static_cast<int>(frames.size())) {
        active_frame_index = 0;
      }
    }
  }
}

void Visualizer::run() {

  // Enable window resizable option
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(starting_width, starting_height, "Raylib + Xmake");

  while (!WindowShouldClose()) {
    if (IsWindowResized()) {
      int width = GetScreenWidth();
      int height = GetScreenHeight();
      std::cout << "New window size: " << width << "x" << height << std::endl;
      // Update each frame's area to match the new window size
      for (const auto &frame_ptr : frames) {
        frame_ptr->setArea(getDefaultArea());
      }

      // Update button positions
      int button_size = header_height - 10;
      // The buttons are squares and right-aligned in the header
      prev_button = Rectangle{
          static_cast<float>(GetScreenWidth() - 10 - 2 * button_size), 5.0f,
          static_cast<float>(button_size), static_cast<float>(button_size)};
      next_button = Rectangle{
          static_cast<float>(GetScreenWidth() - 5 - button_size), 5.0f,
          static_cast<float>(button_size), static_cast<float>(button_size)};
    }

    // Update mouse cursor to default
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    // Update logic
    frames.at(active_frame_index)->update();

    BeginDrawing();
    ClearBackground(background_color);

    // Frame drawing.
    // This is done outside the main drawing block to allow each frame to
    // manage its own drawing context if needed (like cameras, etc.)
    frames.at(active_frame_index)->draw(text_color);

    drawHeader();

    EndDrawing();
  }

  CloseWindow();
}

void Visualizer::addFrame(std::unique_ptr<Frame> frame) {
  frames.push_back(std::move(frame));
}

Rectangle Visualizer::getDefaultArea() {
  int width = GetScreenWidth();
  if (width <= 0) {
    width = starting_width;
  }
  int height = GetScreenHeight() - header_height;
  if (height <= 0) {
    height = starting_height - header_height;
  }
  std::cout << "Default area size: " << width << "x" << height << std::endl;
  return Rectangle{0.0f, static_cast<float>(header_height),
                   static_cast<float>(width), static_cast<float>(height)};
}
