#pragma once
#include <entt/entity/fwd.hpp>

namespace renderer {
namespace string {
  void init();
  void render(entt::registry &registry);
  void clean();
} // namespace string
} // namespace renderer
