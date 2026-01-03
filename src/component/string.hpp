#pragma once

#include <mathtypes.hpp>
#include <string_view>

struct StringComp;

struct StringComp {
  glm::vec2 pos;
  u8 scale;
  u8 count;
  char data[14] {};

  constexpr StringComp(glm::vec2 pos, u8 scale, u8 count, std::string_view str) : pos(pos), scale(scale), count((count < 14 ? count : 14)) {
    for (u8 i = 0; i < this->count; ++i) data[i] = str[i];
  }

  constexpr StringComp(glm::vec2 pos, u8 scale, std::string_view str) : StringComp(pos, scale, static_cast<u8>(str.size()), str) {}
  constexpr StringComp(float x, float y, u8 scale, u8 count, std::string_view str) : StringComp(vec2(x, y), scale, count, str) {}
  constexpr StringComp(float x, float y, u8 scale, std::string_view str) : StringComp(vec2(x, y), scale, str) {}
};
