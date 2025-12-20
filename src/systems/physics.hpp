#pragma once

#include <component/physics.hpp>
#include <component/rect.hpp>
#include <entt/entity/fwd.hpp>

inline void update_position(entt::registry &registry, float delta_t) {
  auto view = registry.view<RectComp, Velocity>();
  view.each([=](auto entity, RectComp &rect, Velocity &vel) { rect.pos += vec2(vel.x, vel.y) * delta_t; });
}
