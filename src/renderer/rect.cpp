#include <component/rect.hpp>
#include <entt/entt.hpp>
#include <gltypes.hpp>
#include <iostream>
#include <logger.hpp>
#include <shader.hpp>

constexpr const char *vert_shader = R"(
#version 330 core
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) in vec2 base_pos;
layout(location = 1) in vec2 pos;
layout(location = 2) in uvec2 scale;

const vec2 dim = vec2(160.0f, 90.0f);

void main() {
  const vec2 world_pos = base_pos * scale + pos;
  gl_Position = vec4(world_pos * 2 / dim - 1, 0.0f, 1.0f);
}
)";

constexpr const char *frag_shader = R"(
#version 330 core
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_explicit_uniform_location : enable

out vec4 color;

void main() {
  color = vec4(1.0f);
}
)";

gl::Buffer<RectComp> vertex(GL_ARRAY_BUFFER);
gl::Array<vec2> base_vertex(GL_ARRAY_BUFFER);
gl::VertexArray vao;
GLuint shader;

namespace renderer {
namespace rect {
  void init() {
    vertex.init();
    base_vertex.init({{1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}});
    vao.init();

    vao.bind();
    vao.bind_buffer(base_vertex, sizeof(vec2));
    vao.add_attib(base_vertex, 0, 2, GL_FLOAT, false, 0);

    vao.bind_buffer(vertex, sizeof(RectComp), 1);
    vao.add_attib(vertex, 1, 2, GL_FLOAT, false, offsetof(RectComp, pos));
    vao.add_iattib(vertex, 2, 2, GL_UNSIGNED_BYTE, offsetof(RectComp, scale));

    shader = link_program(vert_shader, frag_shader);
  }

  void render(entt::registry &registry) {
    auto &storage = registry.storage<RectComp>();
    vertex.update(*storage.raw(), storage.size());

    vao.bind();
    glUseProgram(shader);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, storage.size());
  }

  void clean() {
    vao.destroy();
    vertex.destroy();
    base_vertex.destroy();
    glDeleteProgram(shader);
  }
} // namespace rect
} // namespace renderer
