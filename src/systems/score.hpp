#pragma once

#include <component/paddle.hpp>
#include <component/physics.hpp>
#include <component/rect.hpp>
#include <component/string.hpp>
#include <cstring>
#include <entt/entt.hpp>
#include <game.hpp>
#include <key.hpp>
#include <systems/ball.hpp>

inline bool handle_score(entt::registry &registry, const entt::entity score, const entt::entity ball, const entt::entity win_text) {
  vec2 &ball_pos = registry.get<RectComp>(ball).pos;
  vec2 &ball_vel = registry.get<Velocity>(ball);
  StringComp &str = registry.get<StringComp>(score);

  if (ball_pos.x < 0) {
    str.data[3]++;

    if (str.data[3] > '9') {
      str.data[0] = '0';
      str.data[3] = '0';

      StringComp &win_str = registry.get<StringComp>(win_text);
      memcpy(win_str.data, "WIN RIG", 7);
      win_str.count = 7;
    }

    ball_pos = (SPACE - vec2(BALL_SIZE)) / 2.0f;
    ball_vel = {0, 0};
    return true;

  } else if (ball_pos.x + BALL_SIZE > SPACE.x) {
    str.data[0]++;

    if (str.data[0] > '9') {
      str.data[0] = '0';
      str.data[3] = '0';

      StringComp &win_str = registry.get<StringComp>(win_text);
      memcpy(win_str.data, "LEF WIN", 7);
      win_str.count = 7;
    }

    ball_pos = (SPACE - vec2(BALL_SIZE)) / 2.0f;
    ball_vel = {0, 0};
    return true;
  }

  return 0;
}
