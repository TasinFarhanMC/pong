#include <betr/namespace.hpp>

#include <CLI/CLI.hpp>
#include <GLFW/glfw3.h>
#include <betr/atomic.hpp>
#include <betr/chrono.hpp>
#include <betr/thread.hpp>
#include <glext/gl.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <key.hpp>
#include <logger.hpp>
#include <print>
#include <scenes/scene.hpp>

#include "global.hpp"

extern Scene MainScene;

Scene *node;
GLFWwindow *window;

int exit_code = 0;
Atomic<bool> running = true;

void request_exit(int code) {
  exit_code = code;
  running = false;
}

int main(int argc, char *argv[]) {
  CLI::App cli_app("", "Pong");
  std::string name = "world";
  bool console = false;
  cli_app.add_flag("-c,--console", console, "Echo logger output to console");
  CLI11_PARSE(cli_app, argc, argv);
  if (!logger::init(console)) { std::println("Unable to init logger"); };

  glfwSetErrorCallback([](int error, const char *desc) { LOG_ERROR("Init", "GLFW Error ({}): {}", error, desc); });

  if (!glfwInit()) {
    LOG_ERROR("Init", "Failed to initialize GLFW");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const GLFWvidmode *monitor_info = glfwGetVideoMode(glfwGetPrimaryMonitor());
  window = glfwCreateWindow(100, 100, "Pong", NULL, NULL);
  if (!window) {
    LOG_ERROR("Init", "Failed to create GLFW window");
    glfwTerminate();
    return -1;
  }
  glfwSetWindowSize(window, monitor_info->width * 2 / 3, monitor_info->height * 2 / 3);
  glfwSetWindowPos(window, monitor_info->width / 6, monitor_info->height / 6);

  glfwMakeContextCurrent(window);
  if (!glext_load_gl(glfwGetProcAddress)) {
    LOG_ERROR("Init", "Unable to load GL");
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
    float min = std::min(width / SPACE_WIDTH, height / SPACE_HEIGHT);
    float m_width = min * SPACE_WIDTH;
    float m_height = min * SPACE_HEIGHT;

    gl_viewport(std::abs(width - m_width) / 2, std::abs(height - m_height) / 2, m_width, m_height);
  });

  gl_enable(GL_BLEND);
  gl_blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  key::set_callback(window);

  MainScene.set(&node);
  node->init();

  LOG_INFO("Init", "Starting Loop");
  float delta_t = 0;

  Thread update([&]() {
    auto next = HighResClock::now();

    while (running) {
      node->update(delta_t);

      next += TICK_DUR;
      std::this_thread::sleep_until(next);
    }
  });

  while (running) {
    static float start_t = glfwGetTime(), end_t = 0, delta_t = 0, acc_t = 0;

    gl_clear_color(0.1, 0.2, 0.3, 1.0f);
    gl_clear(GL_COLOR_BUFFER_BIT);

    node->render();

    glfwSwapBuffers(window);

    end_t = glfwGetTime();
    delta_t = end_t - start_t;
    start_t = end_t;
    acc_t += delta_t;

    if (acc_t >= TICK_TIME) { glfwPollEvents(); }
  }

  LOG_INFO("Init", "Clean Up");
  update.join();
  Scene::clean_all();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwSwapInterval(1);
  glfwDestroyWindow(window);
  glfwTerminate();
  logger::clean();

  return exit_code;
}
