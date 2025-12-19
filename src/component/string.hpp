#pragma once

#include <algorithm>
#include <mathtypes.hpp>
#include <string_view>

struct StringComp {
  vec2 pos;
  unsigned char scale;
  unsigned char count;
  char data[14] {};

  constexpr StringComp(vec2 pos, unsigned char scale, std::string_view str) : pos(pos), scale(scale) {
    count = str.size() < 14 ? str.size() : 14;
    std::copy(str.begin(), str.begin() + count, data);
  }

  constexpr StringComp(float x, float y, unsigned char scale, std::string_view str) : pos(x, y), scale(scale) {
    count = str.size() < 14 ? str.size() : 14;
    std::copy(str.begin(), str.begin() + count, data);
  }

  constexpr StringComp(vec2 pos, unsigned char scale, unsigned char count, std::string_view str) : pos(pos), scale(scale), count(count) {
    std::copy(str.begin(), str.begin() + (str.size() < 14 ? str.size() : 14), data);
  }

  constexpr StringComp(float x, float y, unsigned char scale, unsigned char count, std::string_view str) : pos(x, y), scale(scale), count(count) {
    std::copy(str.begin(), str.begin() + (str.size() < 14 ? str.size() : 14), data);
  }
};
