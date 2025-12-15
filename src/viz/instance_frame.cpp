#include "viz/instance_frame.hpp"
#include "hostel.hpp"
#include "raylib.h"
#include "raymath.h"
#include <iostream>

InstanceFrame::InstanceFrame(const std::string &title, const Rectangle &area,
                             const Instance &instance)
    : Frame(title, area), instance(instance) {
  // Initialize camera
  camera.target = {0.0f, 0.0f};
  camera.offset = {area.width / 2.0f, area.height / 2};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  // Settings
  world_scale = 20;
  hostel_radius = 6;
  poi_radius = 4;
  hostel_color = {223, 157, 158, 255};
  POI_color = {146, 169, 233, 255};
}
InstanceFrame::~InstanceFrame() = default;

void InstanceFrame::update() {
  // Add mouse-based camera controls:
  // Dragging with left mouse button to pan
  // Scrolling mouse wheel to zoom in/out
  // Also clamp zoom level to reasonable values and support trackpad scrolling
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    // Set cursor to grabbing hand
    SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
    Vector2 delta = GetMouseDelta();
    camera.target.x -= delta.x / camera.zoom;
    camera.target.y -= delta.y / camera.zoom;
  }
  float wheel = GetMouseWheelMove();

  if (wheel != 0) {
    // 1. Get the world point that is under the mouse NOW
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

    // 2. Apply the zoom
    camera.zoom += wheel * 0.125f; // (Changed to 0.125f for cleaner steps)

    // Clamp the zoom
    if (camera.zoom < 1.0f)
      camera.zoom = 1.0f;
    if (camera.zoom > 10.0f)
      camera.zoom = 10.0f;

    // 3. Get the world point that is under the mouse AFTER the zoom
    //    (Note: Since we haven't moved the camera target yet, this point
    //     will be different from step 1, which causes the "drift")
    Vector2 mouseWorldPosNew = GetScreenToWorld2D(GetMousePosition(), camera);

    // 4. Adjust the camera target to compensate for the drift
    //    We effectively "slide" the world back so the original point matches
    //    the mouse again.
    camera.target.x += (mouseWorldPos.x - mouseWorldPosNew.x);
    camera.target.y += (mouseWorldPos.y - mouseWorldPosNew.y);
  }
  // Figure out hovered point. We will then use it to draw a cool pop-up.
  hovered_point_index = -1;
  const WorldMap &world_map = instance.getWorldMap();
  // First check POIs
  for (int i = 0; i < world_map.getPOICount(); i++) {
    const POI &p = world_map.getPOIByIndex(i);
    Vector2 poi_screen_pos = GetWorldToScreen2D(
        {p.getX() * world_scale, p.getY() * world_scale}, camera);
    float distance = Vector2Distance(poi_screen_pos, GetMousePosition());
    if (distance <= poi_radius * camera.zoom) {
      hovered_point_index = i;
      break;
    }
  }

  // Then check Hostels if no POI is hovered
  if (hovered_point_index == -1) {
    for (int i = 0; i < world_map.getHostelCount(); i++) {
      const Hostel &h = world_map.getHostelByIndex(i);
      Vector2 hostel_screen_pos = GetWorldToScreen2D(
          {h.getX() * world_scale, h.getY() * world_scale}, camera);
      float distance = Vector2Distance(hostel_screen_pos, GetMousePosition());
      if (distance <= hostel_radius * camera.zoom) {
        hovered_point_index = i + world_map.getPOICount();
        break;
      }
    }
  }

  // If any POI or Hostel is hovered, set cursor to pointer
  if (hovered_point_index != -1) {
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  } else {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }
}

void InstanceFrame::draw(const Color &text_color) {
  // Call base class draw method
  // Frame::draw(text_color);

  // Use the camera
  BeginMode2D(camera);

  // Draw each point
  const WorldMap &world_map = instance.getWorldMap();
  // First draw hostels
  for (int i = 0; i < world_map.getHostelCount(); i++) {
    const Hostel &h = world_map.getHostelByIndex(i);
    // If the hostel is currently hovered, draw it larger
    int radius = hostel_radius;
    if (hovered_point_index == i + world_map.getPOICount()) {
      radius += 1;
    }
    DrawCircle(static_cast<int>(h.getX() * world_scale),
               static_cast<int>(h.getY() * world_scale), radius, hostel_color);
    DrawText(h.getLabel().c_str(),
             static_cast<int>(h.getX() * world_scale) + 10,
             static_cast<int>(h.getY() * world_scale) - 10, 8, hostel_color);
  }

  // Then draw POIs
  for (int i = 0; i < world_map.getPOICount(); i++) {
    const POI &p = world_map.getPOIByIndex(i);
    // If the POI is currently hovered, draw it larger
    int radius = poi_radius;
    if (hovered_point_index == i) {
      radius += 1;
    }
    DrawCircle(static_cast<int>(p.getX() * world_scale),
               static_cast<int>(p.getY() * world_scale), radius, POI_color);
    DrawText(p.getLabel().c_str(), static_cast<int>(p.getX() * world_scale) + 7,
             static_cast<int>(p.getY() * world_scale) - 7, 8, POI_color);
  }

  // End camera mode
  EndMode2D();

  // Debug info about hovered point
  if (hovered_point_index != -1) {
    std::string info_text;
    if (hovered_point_index < world_map.getPOICount()) {
      // It's a POI
      const POI &p = world_map.getPOIByIndex(hovered_point_index);
      info_text = "POI: " + p.getLabel() + "\n(" + std::to_string(p.getX()) +
                  ", " + std::to_string(p.getY()) + ")";
    } else {
      // It's a Hostel
      int hostel_index = hovered_point_index - world_map.getPOICount();
      const Hostel &h = world_map.getHostelByIndex(hostel_index);
      info_text = "Hostel: " + h.getLabel() + "\n(" + std::to_string(h.getX()) +
                  ", " + std::to_string(h.getY()) + ")";
    }
    DrawText(info_text.c_str(), 10, 30, 12, text_color);
  } else {
    DrawText("No point hovered", 10, 30, 12, text_color);
  }

  // Ultimately, we want to draw a nice pop-up near the mouse cursor.
  // We will first generate all the text we want to render, then measure the
  // size of the largest text line to compute the size of the pop-up box. We
  // will then be able to decide where to position the box so it doesn't go off
  // the screen.
  if (hovered_point_index != -1) {
    std::vector<std::string> info_lines;
    if (hovered_point_index < world_map.getPOICount()) {
      // It's a POI
      info_lines.push_back("POI Information:");
      const POI &point = world_map.getPOIByIndex(hovered_point_index);
      info_lines.push_back("Label: " + point.getLabel());
      info_lines.push_back("Coordinates: (" + std::to_string(point.getX()) +
                           ", " + std::to_string(point.getY()) + ")");
      info_lines.push_back("Score: " + std::to_string(point.getScore()));
      info_lines.push_back("Visit Duration: " +
                           std::to_string(point.getVisitDuration()) + " hours");
      info_lines.push_back(
          "Opening Hours: " + std::to_string(point.getOpeningTime()) + " to " +
          std::to_string(point.getClosingTime()));

    } else if (hovered_point_index >= world_map.getPOICount()) {
      // It's a Hostel
      info_lines.push_back("Hostel Information:");
      int hostel_index = hovered_point_index - world_map.getPOICount();
      const Hostel &hostel = world_map.getHostelByIndex(hostel_index);
      info_lines.push_back("Label: " + hostel.getLabel());
      info_lines.push_back("Coordinates: (" + std::to_string(hostel.getX()) +
                           ", " + std::to_string(hostel.getY()) + ")");
      // Is this hostel the starting or ending hostel?
      if (hostel_index == world_map.getStartingHostelIndex()) {
        info_lines.push_back("Type: Starting Hostel");
      } else if (hostel_index == world_map.getEndingHostelIndex()) {
        info_lines.push_back("Type: Ending Hostel");
      } else {
        info_lines.push_back("Type: Regular Hostel");
      }
    }

    // Compute the largest text width
    int max_text_width = 0;
    for (const std::string &line : info_lines) {
      int line_width = MeasureText(line.c_str(), 12);
      if (line_width > max_text_width) {
        max_text_width = line_width;
      }
    }

    // Compute the pop-up height
    int popup_height = static_cast<int>(info_lines.size()) * 14 + 10;

    // Decide pop-up position based on mouse position
    Vector2 mouse_pos = GetMousePosition();
    int popup_x = static_cast<int>(mouse_pos.x) + 15;
    int popup_y = static_cast<int>(mouse_pos.y) + 15;
    if (popup_x + max_text_width + 10 > GetScreenWidth()) {
      popup_x = static_cast<int>(mouse_pos.x) - (max_text_width + 25);
    }
    if (popup_y + popup_height > GetScreenHeight()) {
      popup_y = static_cast<int>(mouse_pos.y) - (popup_height + 5);
    }

    // Draw the pop-up background
    DrawRectangle(popup_x, popup_y, max_text_width + 10, popup_height,
                  {50, 50, 50, 220});
    DrawRectangleLines(popup_x, popup_y, max_text_width + 10, popup_height,
                       {200, 200, 200, 255});
    // Draw each line of text
    for (size_t i = 0; i < info_lines.size(); i++) {
      DrawText(info_lines[i].c_str(), popup_x + 5,
               popup_y + 5 + static_cast<int>(i) * 14, 12, text_color);
    }
  }
}