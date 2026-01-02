#include "key.hpp"
#include <GLFW/glfw3.h>
// #include <imgui_impl_glfw.h>

using namespace key;

static GLFWwindow *window;
static Event events[GLFW_KEY_LAST + 1];

namespace key {
void set_callback(GLFWwindow *window) {
  ::window = window;

  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
    events[key] = static_cast<Event>(action);
    // ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
  });
}

bool get(int key) { return glfwGetKey(window, key); }

bool get_event(Event event, int key) {
  if (events[key] != event) { return false; }

  events[key] = Event::Expired;
  return true;
};
} // namespace key
