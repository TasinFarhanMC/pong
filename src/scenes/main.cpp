#include <global.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <key.hpp>
#include <logger.hpp>

#include "scene.hpp"

bool init();
void render(float);
void update(float delta_time);
void clean();

Scene main_scene {init, render, update, clean};
static Scene *that = &main_scene;

bool init() { return true; }

void render(float delta_time) {
  ImGui_ImplGlfw_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};

void update(float delta_time) {
  if (key::get(GLFW_KEY_ESCAPE)) { request_exit(0); }
};

void clean() {};
