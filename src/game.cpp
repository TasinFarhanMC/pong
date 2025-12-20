#include <component/physics.hpp>
#include <component/rect.hpp>
#include <component/string.hpp>
#include <entt/entt.hpp>
#include <game.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <key.hpp>
#include <renderer/rect.hpp>
#include <renderer/string.hpp>
#include <systems/paddle.hpp>
#include <systems/physics.hpp>

static entt::registry registry;
static entt::entity ball, score;
static entt::entity paddles[2];

void init() {
  ball = registry.create();
  registry.emplace<RectComp>(ball, (SPACE - vec2(BALL_SIZE)) / 2.0f, u8vec2(BALL_SIZE));
  registry.emplace<Velocity>(ball, 0, 0);

  for (auto &entity : paddles) {
    entity = registry.create();

    registry.emplace<Velocity>(entity, 0, 0);
    registry.emplace<paddle::Collision>(entity, 0);
  }

  registry.emplace<RectComp>(paddles[0], PADDLE_POS, PADDLE_SIZE);
  registry.emplace<RectComp>(paddles[1], vec2(SPACE.x - (PADDLE_POS.x + PADDLE_SIZE.x), PADDLE_POS.y), PADDLE_SIZE);

  for (int i = 0; i < 28; i += 2) {
    const auto entity = registry.create();
    const u8vec2 size = {1, 3};
    registry.emplace<RectComp>(entity, vec2((SPACE.x - size.x) / 2, i * size.y), size);
  }

  constexpr u8 score_size = 12;
  constexpr float score_top_padding = 2;

  score = registry.create();
  registry.emplace<StringComp>(score, vec2((SPACE.x - (score_size * 4)) / 2, SPACE.y - (score_size + score_top_padding)), 12, "0  0");

  renderer::rect::init();
  renderer::string::init();
}

void render(float delta_t) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  handle_paddle_collision(registry, paddles, delta_t);
  update_position(registry, delta_t);

  renderer::rect::render(registry);
  renderer::string::render(registry);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void update(float delta_t) { handle_paddle_input(registry, paddles); }

void clean() {
  registry.clear();
  renderer::rect::clean();
  renderer::string::clean();
}
