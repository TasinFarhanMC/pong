#include <CLI/CLI.hpp>
#include <GLFW/glfw3.h>
#include <chrono>
#include <game.hpp>
#include <glad/gl.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <key.hpp>
#include <logger.hpp>
#include <print>

using namespace std::chrono_literals;

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
  GLFWwindow *window = glfwCreateWindow(100, 100, "Pong", NULL, NULL);

  if (!window) {
    LOG_ERROR("Init", "Failed to create GLFW window");
    glfwTerminate();
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
    float min = std::min(width / SPACE_WIDTH, height / SPACE_HEIGHT);
    float m_width = min * SPACE_WIDTH;
    float m_height = min * SPACE_HEIGHT;

    glViewport(std::abs(width - m_width) / 2, std::abs(height - m_height) / 2, m_width, m_height);
  });

  glfwSetWindowSize(window, monitor_info->width * 2 / 3, monitor_info->height * 2 / 3);
  glfwSetWindowPos(window, monitor_info->width / 6, monitor_info->height / 6);

  glfwMakeContextCurrent(window);
  if (!gladLoadGL(glfwGetProcAddress)) {
    LOG_ERROR("Init", "Unable to load GL");
    return -1;
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  key::set_callback(window);
  glfwSwapInterval(0);

  init();

  LOG_INFO("Init", "Starting Loop");
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
    delta_t = (now - start).count();
    start = now;
    accumilator += delta_t;

    if (accumilator >= TICK_TIME) {
      if (key::get_event(key::Event::Press, GLFW_KEY_ESCAPE)) { glfwSetWindowShouldClose(window, true); }
      update(delta_t);
    }
  }

  LOG_INFO("Init", "Clean Up");

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwSwapInterval(1);
  glfwDestroyWindow(window);
  glfwTerminate();
  logger::clean();
}
