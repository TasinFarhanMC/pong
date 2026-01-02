#include <GLFW/glfw3.h>
#include <chrono>
#include <game.hpp>
#include <glad/gl.h>
#include <iostream>
#include <key.hpp>
#include <logger.hpp>

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
  bool info = false, verbose = false;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "-c" || arg == "--console") { info = true; }
    if (arg == "-v" || arg == "--verbose") { verbose = true; }
  }

  if (!logger::init(info, verbose)) { std::cerr << "Unable to init logger" << std::endl; };

  glfwSetErrorCallback([](int error, const char *desc) { LOG_ERROR("Window", "GLFW Error ({}): {}", error, desc); });

  if (!glfwInit()) {
    LOG_ERROR("Window", "Failed to initialize GLFW");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const GLFWvidmode *monitor_info = glfwGetVideoMode(glfwGetPrimaryMonitor());
  GLFWwindow *window = glfwCreateWindow(100, 100, "Pong", NULL, NULL);

  if (!window) {
    LOG_ERROR("Window", "Failed to create GLFW window");
    glfwTerminate();
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
    float min = std::min(width / SPACE.x, height / SPACE.y);
    float m_width = min * SPACE.x;
    float m_height = min * SPACE.y;

    glViewport(std::abs(width - m_width) / 2, std::abs(height - m_height) / 2, m_width, m_height);
  });

  glfwSetWindowSize(window, monitor_info->width * 2 / 3, monitor_info->height * 2 / 3);
  glfwSetWindowPos(window, monitor_info->width / 6, monitor_info->height / 6);

  glfwMakeContextCurrent(window);
  if (!gladLoadGL(glfwGetProcAddress)) {
    LOG_ERROR("Graphics/GL", "Unable to load GL");
    return -1;
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  key::set_callback(window);
  // glfwSwapInterval(0);

  init();

  LOG_INFO("Game", "Starting Loop");
  float delta_t = 0;
  float accumilator = 0;

  auto start = std::chrono::high_resolution_clock::now();

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1, 0.2, 0.3, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    render(delta_t);

    glfwSwapBuffers(window);
    glfwPollEvents();

    auto now = std::chrono::high_resolution_clock::now();
    delta_t = float((now - start).count()) / std::nano::den;
    start = now;
    accumilator += delta_t;

    if (accumilator >= TICK_TIME) {
      if (key::get_event(key::Event::Press, GLFW_KEY_ESCAPE)) { glfwSetWindowShouldClose(window, true); }
      update();
      accumilator -= TICK_TIME;
    }
  }

  LOG_INFO("Game", "Clean Up");

  glfwSwapInterval(1);
  glfwDestroyWindow(window);
  glfwTerminate();
  logger::clean();
}
