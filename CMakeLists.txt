cmake_minimum_required(VERSION 3.11)
project(PONG C CXX)

# Make Cmake Use New OpenGL
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/get_cpm.cmake)
cpmaddpackage(URI "gh:glfw/glfw#3.4" OPTIONS "GLM_ENABLE_SIMD_SSE4_2 ON"
              "GLM_ENABLE_CXX_23 ON")
cpmaddpackage("gh:g-truc/glm#1.0.1")
cpmaddpackage("gh:tasinfarhanmc/betr#main")
cpmaddpackage("gh:g-truc/glm#1.0.1")
cpmaddpackage("gh:tasinfarhanmc/glext@3.3.2")
cpmaddpackage("gh:CLIUtils/CLI11@2.5.0")
cpmaddpackage("gh:skypjack/entt@3.15.0")
cpmaddpackage(
  NAME
  Imgui
  GIT_REPOSITORY
  "https://github.com/ocornut/imgui"
  VERSION
  1.92.1
  DOWNLOAD_ONLY)

add_library(
  imgui STATIC
  ${Imgui_SOURCE_DIR}/imgui.cpp
  ${Imgui_SOURCE_DIR}/imgui_demo.cpp
  ${Imgui_SOURCE_DIR}/imgui_draw.cpp
  ${Imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${Imgui_SOURCE_DIR}/imgui_tables.cpp
  ${Imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${Imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)

target_include_directories(imgui PUBLIC ${Imgui_SOURCE_DIR}
                                        ${Imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw)

# TODO: DOWNLOAD IMAGE LOADER

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIRS "${SRC_DIR}")

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c" "${SRC_DIR}/*.cpp")

add_executable(pong "${SRC_FILES}")
target_include_directories(pong PRIVATE ${INCLUDE_DIRS})
target_link_libraries(
  pong
  PRIVATE OpenGL::GL
          glm
          glfw
          imgui
          betr
          glext
          CLI11
          EnTT)
target_compile_definitions(pong PRIVATE GLFW_INCLUDE_NONE)
target_compile_features(pong PRIVATE cxx_std_23)
set_target_properties(pong PROPERTIES CXX_SCAN_FOR_MODULES OFF)
