#pragma once
#include <chrono>

constexpr int TICK_RATE = 30;
constexpr float TICK_TIME = 1.0f / TICK_RATE;
constexpr std::chrono::nanoseconds TICK_DUR {std::nano::den / TICK_RATE};

constexpr float SPACE_HEIGHT = 90.0f;
constexpr float SPACE_WIDTH = 160.0f;

void init();
void render(float delta_t);
void update(float delta_t);
void clean();
