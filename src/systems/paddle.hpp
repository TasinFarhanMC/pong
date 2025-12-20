#pragma once

#include <component/paddle.hpp>
#include <component/physics.hpp>
#include <component/rect.hpp>
#include <entt/entt.hpp>
#include <game.hpp>
#include <key.hpp>

inline void handle_paddle_input(entt::registry &registry, const entt::entity *paddles) {
  auto &vel0 = registry.get<Velocity>(paddles[0]).y;
  auto &col0 = registry.get<paddle::Collision>(paddles[0]).direction;
  auto &vel1 = registry.get<Velocity>(paddles[1]).y;
  auto &col1 = registry.get<paddle::Collision>(paddles[1]).direction;

  bool w = key::get(GLFW_KEY_W);
  bool s = key::get(GLFW_KEY_S);

  char dir = (w << 1) - 1;
  if (w == s || col0 == dir) {
    vel0 = 0;
  } else {
    vel0 = dir * PADDLE_VEL_Y;
    col0 = 0;
  }

  bool up = key::get(GLFW_KEY_UP);
  bool down = key::get(GLFW_KEY_DOWN);

  dir = (up << 1) - 1;
  if (up == down || col1 == dir) {
    vel1 = 0;
  } else {
    vel1 = dir * PADDLE_VEL_Y;
    col1 = 0;
  }
}

inline void handle_paddle_collision(entt::registry &registry, const std::span<entt::entity> paddles, float delta_t) {
  for (auto &entity : paddles) {
    auto &coll = registry.get<paddle::Collision>(entity).direction;
    if (coll != 0) { continue; }

    auto &pos = registry.get<RectComp>(entity).pos;
    auto &vel = registry.get<Velocity>(entity);

    float t1 = -pos.y / vel.y;
    float t2 = (SPACE.y - (pos.y + PADDLE_SIZE.y)) / vel.y;

    if (t1 > 0 && t1 < delta_t) {
      coll = -1;
      vel.y = 0;
      pos.y = 0;
    } else if (t2 > 0 && t2 < delta_t) {
      coll = 1;
      vel.y = 0;
      pos.y = SPACE.y - PADDLE_SIZE.y;
    }
  }
}
