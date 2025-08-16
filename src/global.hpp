#pragma once
#include <betr/chrono.hpp>

void request_exit(int code);

constexpr int TICK_RATE = 30;
constexpr float TICK_TIME = 1.0f / TICK_RATE;
constexpr betr::NanoSeconds TICK_DUR {betr::Nano::den / TICK_RATE};

constexpr float SPACE_HEIGHT = 90.0f;
constexpr float SPACE_WIDTH = 160.0f;
