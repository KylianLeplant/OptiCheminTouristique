#pragma once
#include "raylib.h"
#include <string>

class Frame {
private:
  std::string frame_title;
  Rectangle frame_area;

public:
  Frame(const std::string &title, const Rectangle &area);
  virtual ~Frame();

  const std::string &getTitle() const;
  void setArea(const Rectangle &area);
  virtual void update();
  virtual void draw(const Color &text_color);
};