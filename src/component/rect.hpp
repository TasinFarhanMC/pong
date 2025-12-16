#pragma once
#include <mathtypes.hpp>

struct RectComp {
  vec2 pos;
  u8vec2 scale;

  RectComp(const vec2 &p, const u8vec2 &s) : pos(p), scale(s) {}
  RectComp(float x, float y, glm::u8 w, glm::u8 h) : pos(x, y), scale(w, h) {}
};
