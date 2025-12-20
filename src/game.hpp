#pragma once
#include "mathtypes.hpp"
#include <chrono>

constexpr int TICK_RATE = 30;
constexpr float TICK_TIME = 1.0f / TICK_RATE;
constexpr std::chrono::nanoseconds TICK_DUR {std::nano::den / TICK_RATE};

constexpr vec2 SPACE = {160, 90};

constexpr u8 BALL_SIZE = 3;
constexpr u8vec2 PADDLE_SIZE = {4, 20};
constexpr vec2 PADDLE_POS = {10, (SPACE.y - PADDLE_SIZE.y) / 2};
constexpr float PADDLE_VEL_Y = 200.0f;

void init();
void render(float delta_t);
void update(float delta_t);
void clean();
