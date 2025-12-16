#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "key.hpp"
#include <component/physics.hpp>
#include <component/rect.hpp>
#include <entt/entt.hpp>
#include <game.hpp>
#include <imgui.h>
#include <renderer/rect.hpp>

entt::registry registry;
entt::entity ball;
entt::entity paddle[2];

struct Collision {
  char direction;
};

bool Vec2Picker(const char *label, vec2 size, vec2 *value, ImVec2 min, ImVec2 max) {
  ImGui::TextUnformatted(label);

  ImVec2 p0 = ImGui::GetCursorScreenPos();
  ImVec2 p1 = {p0.x + size.x, p0.y + size.y};

  ImGui::InvisibleButton(label, ImVec2(size.x, size.y));
  bool changed = ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left);

  if (changed) {
    ImVec2 mouse = ImGui::GetIO().MousePos;

    float tx = (mouse.x - p0.x) / size.x;
    float ty = 1.0f - (mouse.y - p0.y) / size.y;

    tx = ImClamp(tx, 0.0f, 1.0f);
    ty = ImClamp(ty, 0.0f, 1.0f);

    value->x = min.x + tx * (max.x - min.x);
    value->y = min.y + ty * (max.y - min.y);
  }

  ImDrawList *dl = ImGui::GetWindowDrawList();

  // background
  dl->AddRectFilled(p0, p1, IM_COL32(40, 40, 40, 255));

  // grid
  for (int i = 1; i < 10; ++i) {
    float x = p0.x + size.x * i / 10.0f;
    float y = p0.y + size.y * i / 10.0f;
    dl->AddLine({x, p0.y}, {x, p1.y}, IM_COL32(60, 60, 60, 255));
    dl->AddLine({p0.x, y}, {p1.x, y}, IM_COL32(60, 60, 60, 255));
  }

  // marker
  float nx = (value->x - min.x) / (max.x - min.x);
  float ny = (value->y - min.y) / (max.y - min.y);

  ImVec2 marker = {p0.x + nx * size.x, p0.y + (1.0f - ny) * size.y};

  dl->AddCircleFilled(marker, 4.0f, IM_COL32(255, 100, 100, 255));

  return changed;
}

void update_pos(entt::registry &registry, float delta_t) {
  auto view = registry.view<RectComp, Velocity>();

  view.each([=](auto entity, RectComp &rect, Velocity &vel) { rect.pos += vec2(vel.x, vel.y) * delta_t; });
}

void paddle_collision(entt::registry &registry, float delta_t) {
  for (auto &entity : paddle) {
    auto &coll = registry.get<Collision>(entity).direction;
    if (coll != 0) { continue; }

    auto &rect = registry.get<RectComp>(entity);
    auto &vel = registry.get<Velocity>(entity);

    float t1 = -rect.pos.y / vel.y;
    float t2 = (SPACE_HEIGHT - (rect.pos.y + rect.scale.y)) / vel.y;

    if (t1 > 0 && t1 < delta_t) {
      coll = -1;
      vel.y = 0;
      rect.pos.y = 0;
    } else if (t2 > 0 && t2 < delta_t) {
      coll = 1;
      vel.y = 0;
      rect.pos.y = SPACE_HEIGHT - rect.scale.y;
    }
  }
}

void init() {
  ball = registry.create();
  registry.emplace<RectComp>(ball, 78.5f, 43.5f, 3, 3);
  registry.emplace<Velocity>(ball, 0, 0);

  for (auto &entity : paddle) {
    entity = registry.create();
    registry.emplace<RectComp>(entity, 10, SPACE_HEIGHT / 2 - 15, 4, 30);
    registry.emplace<Velocity>(entity, 0, 0);
    registry.emplace<Collision>(entity, 0);
  }

  auto &paddle1 = registry.get<RectComp>(paddle[1]);
  paddle1.pos.x = SPACE_WIDTH - (paddle1.pos.x + paddle1.scale.x);

  renderer::rect::init();
}

void render(float delta_t) {
  // 1. Start new ImGui frame **first**
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // 2. ImGui widgets
  auto &rect = registry.get<RectComp>(paddle[0]);
  vec2 v = {rect.pos.x, rect.pos.y};
  if (Vec2Picker("Position", {160, 90}, &v, {0, 0}, {float(160 - rect.scale.x), float(90 - rect.scale.y)})) { rect.pos = v; }

  if (ImGui::InputFloat2("Position Val", &v[0])) { rect.pos = v; }

  ivec2 scale = rect.scale;
  ImGui::SliderInt2("Scale", &scale[0], 1, 255);
  rect.scale = scale;

  auto &vel = registry.get<Velocity>(paddle[0]);
  ImGui::DragFloat2("Velocity", &vel[0]);

  // 3. Render your game objects
  paddle_collision(registry, delta_t);
  update_pos(registry, delta_t);

  renderer::rect::render(registry);

  // 4. Render ImGui
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void update(float delta_t) {
  using namespace key;

  auto &vel0 = registry.get<Velocity>(paddle[0]).y;
  auto &col0 = registry.get<Collision>(paddle[0]).direction;

  auto &vel1 = registry.get<Velocity>(paddle[1]).y;
  auto &col1 = registry.get<Collision>(paddle[1]).direction;

  bool w = get(GLFW_KEY_W);
  bool s = get(GLFW_KEY_S);
  char dir = w * 2 - 1;

  if (w == s || col0 == dir) {
    vel0 = 0;
  } else {
    vel0 = dir * 150;
    col0 = 0;
  }

  bool up = get(GLFW_KEY_UP);
  bool down = get(GLFW_KEY_DOWN);
  dir = up * 2 - 1;

  if (up == down || col1 == dir) {
    vel1 = 0;
  } else {
    vel1 = dir * 150;
    col1 = 0;
  }
}

void clean() {
  registry.clear();
  renderer::rect::clean();
}
