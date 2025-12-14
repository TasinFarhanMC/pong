#pragma once
#include <entt/entity/fwd.hpp>

namespace renderer {
namespace rect {
  void init();
  void render(entt::registry &registry); // namespace rect
  void clean();
} // namespace rect
} // namespace renderer
