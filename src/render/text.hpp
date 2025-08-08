#pragma once
#include <betr/glm/ext/vec_float2.hpp>
#include <betr/glm/ext/vec_uint2.hpp>
#include <betr/string.hpp>
#include <format>
#include <gl.hpp>

class Text {
public:
  struct Vertex {
    betr::Vec2 pos;
    betr::UVec2 scale;
    unsigned char id;

    Vertex(float x, float y, unsigned w, unsigned h, unsigned char id) : pos(x, y), scale(w, h), id(id) {}
    Vertex() {}
  };

  Text(betr::InitList<Vertex> data)
      : vertex(GL_ARRAY_BUFFER, data),
        base_vertex(GL_ARRAY_BUFFER, {{1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}}) {
    count = data.size();
    init();
  };

  void println(float x, float y, betr::String s);

  template <typename... Args> inline void println(float x, float y, std::format_string<Args...> fmt, Args &&...args) {
    println(x, y, std::format(fmt, std::forward<Args>(args)...));
  }

  void render(gl::Pipeline &pipeline);

  Text(Text &&) = delete;
  Text(const Text &) = delete;
  Text &operator=(Text &&) = delete;
  Text &operator=(const Text &) = delete;

private:
  gl::Buffer<Vertex> vertex;
  gl::Array<betr::Vec2> base_vertex;
  gl::VertexArray vao;

  GLuint texture;
  int count;

  void init();
};
