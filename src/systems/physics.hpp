#pragma once

#include <component/physics.hpp>
#include <component/rect.hpp>
#include <entt/entt.hpp>
#include <game.hpp>

inline void update_position(entt::registry &registry, float delta_t) {
  auto view = registry.view<RectComp, Velocity>();
  view.each([=](auto entity, RectComp &rect, Velocity &vel) { rect.pos += vel * delta_t; });
}

inline void handle_ball_wall_collision(entt::registry &registry, entt::entity ball) {
  float &vel_y = registry.get<Velocity>(ball).y;
  float &pos_y = registry.get<RectComp>(ball).pos.y;

  float next_y = pos_y + vel_y * TICK_TIME;

  if (next_y + BALL_SIZE > SPACE.y) {
    vel_y *= -1;
  } else if (next_y < 0) {
    vel_y *= -1;
  }
}

inline void handle_ball_paddle_collision(entt::registry &registry, entt::entity ball, const std::span<entt::entity> paddles) {
  vec2 &ball_pos = registry.get<RectComp>(ball).pos;
  vec2 &ball_vel = registry.get<Velocity>(ball);
  const vec2 &ball_size = registry.get<RectComp>(ball).scale;

  if (ball_vel.x == 0 && ball_vel.y == 0) { return; }

  for (auto &paddle : paddles) {
    const vec2 &paddle_pos = registry.get<RectComp>(paddle).pos;
    const vec2 &paddle_size = registry.get<RectComp>(paddle).scale;
    const vec2 &paddle_vel = registry.get<Velocity>(paddle);

    vec2 rel_disp = (ball_vel - paddle_vel) * TICK_TIME;
    if (rel_disp.x == 0.0f && rel_disp.y == 0.0f) continue;

    float x_inv_entry = rel_disp.x > 0.0f ? paddle_pos.x - (ball_pos.x + ball_size.x) : (paddle_pos.x + paddle_size.x) - ball_pos.x;
    float x_inv_exit = rel_disp.x > 0.0f ? (paddle_pos.x + paddle_size.x) - ball_pos.x : paddle_pos.x - (ball_pos.x + ball_size.x);
    float y_inv_entry = rel_disp.y > 0.0f ? paddle_pos.y - (ball_pos.y + ball_size.y) : (paddle_pos.y + paddle_size.y) - ball_pos.y;
    float y_inv_exit = rel_disp.y > 0.0f ? (paddle_pos.y + paddle_size.y) - ball_pos.y : paddle_pos.y - (ball_pos.y + ball_size.y);

    float x_entry = rel_disp.x == 0.0f ? -INFINITY : x_inv_entry / rel_disp.x;
    float x_exit = rel_disp.x == 0.0f ? INFINITY : x_inv_exit / rel_disp.x;
    float y_entry = rel_disp.y == 0.0f ? -INFINITY : y_inv_entry / rel_disp.y;
    float y_exit = rel_disp.y == 0.0f ? INFINITY : y_inv_exit / rel_disp.y;

    float entry_time = std::max(x_entry, y_entry);
    float exit_time = std::min(x_exit, y_exit);

    if (entry_time >= 0.0f && entry_time <= 1.0f && entry_time <= exit_time) {
      ball_pos += rel_disp * entry_time;

      if (x_entry > y_entry) {
        ball_vel.x *= -1.0f;

      } else {
        ball_vel.y *= -1.0f;
      }
    }
  }
}
