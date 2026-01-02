#pragma once

#include <component/physics.hpp>
#include <component/rect.hpp>
#include <entt/entt.hpp>
#include <game.hpp>
#include <random>

static std::mt19937 gen(std::random_device {}());

inline void set_ball_initial_vel(entt::registry &registry, entt::entity ball) {
  constexpr float SPEED = 100;
  constexpr float SPREAD = 90;
  constexpr float sigma = SPREAD / 3 * std::numbers::pi_v<float> / 180;

  std::normal_distribution<float> angle_dist(0, sigma);
  float angle = angle_dist(gen);

  std::uniform_int_distribution<int> direction_dist(0, 1);
  bool right_side = direction_dist(gen);

  float x = std::cos(angle) * SPEED * ((right_side) ? 1 : -1);
  float y = std::sin(angle) * SPEED;

  registry.get<Velocity>(ball) = {x, y};
}
