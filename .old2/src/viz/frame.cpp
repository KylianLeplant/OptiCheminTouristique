#include "viz/frame.hpp"
#include "raylib.h"

Frame::Frame(const std::string &title, const Rectangle &area)
    : frame_title(title), frame_area(area) {}

Frame::~Frame() = default;

const std::string &Frame::getTitle() const { return frame_title; }
void Frame::setArea(const Rectangle &area) { frame_area = area; }
void Frame::update() {}
void Frame::draw(const Color &text_color) {

  // The default draw method only displays the frame area as a string (position
  // and size)
  std::string info =
      "Area : " + std::to_string(static_cast<int>(frame_area.width)) + "x" +
      std::to_string(static_cast<int>(frame_area.height)) + " at {" +
      std::to_string(static_cast<int>(frame_area.x)) + ", " +
      std::to_string(static_cast<int>(frame_area.y)) + "}";
  DrawText(info.c_str(), static_cast<int>(frame_area.x) + 10,
           static_cast<int>(frame_area.y) + 10, 20, text_color);
}