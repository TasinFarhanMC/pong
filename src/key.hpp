#pragma once
#include <GLFW/glfw3.h>

namespace key {
enum class Event : char { Release = GLFW_RELEASE, Press = GLFW_PRESS, Repeat = GLFW_REPEAT, Expired };

void set_callback(GLFWwindow *window);
bool get(int key);
bool get_event(Event event, int key);

} // namespace key
