#include "component/rect.hpp"
#include <entt/entt.hpp>
#include <game.hpp>
#include <renderer/rect.hpp>

entt::registry registry;
entt::entity box;

void init() {
  box = registry.create();
  registry.emplace<component::Rect>(box, vec2 {50.0f, 50.0f}, u8vec2 {20});
  registry.emplace<component::Rect>(registry.create(), vec2 {100, 60}, u8vec2 {40});

  renderer::rect::init();
}

void render(float delta_t) { renderer::rect::render(registry); }

void update(float delta_t) {}

void clean() {
  registry.clear();
  renderer::rect::clean();
}
