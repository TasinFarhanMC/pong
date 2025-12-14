#pragma once

#include <glad/gl.h>
#include <initializer_list>
#include <type_traits>

namespace gl {
template <typename T> class Buffer {
public:
  Buffer(GLenum type) : type(type) {};
  void destroy() { glDeleteBuffers(1, &id); }

  operator GLuint() const noexcept { return this->id; }

  void init(const std::initializer_list<T> data = {}) {
    glGenBuffers(1, &id);
    glBindBuffer(type, id);
    glBufferData(type, data.size() * sizeof(T), data.begin(), GL_DYNAMIC_DRAW);
  }

  T *map() {
    glBindBuffer(type, id);
    return static_cast<T *>(glMapBuffer(type, GL_READ_WRITE));
  }
  void unmap() {
    glBindBuffer(type, id);
    glUnmapBuffer(type);
  }

  void update(const T *data, int size) {
    glBindBuffer(type, id);
    glBufferData(type, sizeof(T) * size, data, GL_DYNAMIC_DRAW);
  }

private:
  const GLenum type;
  GLuint id;
};

template <typename T> class Array {
public:
  Array(GLenum type) : type(type) {};
  void destroy() { glDeleteBuffers(1, &id); }

  operator GLuint() const noexcept { return this->id; }

  void init(const std::initializer_list<T> data) {
    glGenBuffers(1, &id);
    glBindBuffer(type, id);
    glBufferData(type, data.size() * sizeof(T), data.begin(), GL_STATIC_DRAW);
  }

  T *map() {
    glBindBuffer(type, id);
    return static_cast<T *>(glMapBuffer(type, GL_READ_ONLY));
  }
  void unmap() {
    glBindBuffer(type, id);
    glUnmapBuffer(type);
  }

  Array(Array &&) = delete;
  Array(const Array &) = delete;
  Array &operator=(Array &&) = delete;
  Array &operator=(const Array &) = delete;

private:
  const GLenum type;
  GLuint id;
};

class VertexArray {
public:
  void init() {
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
  };

  void bind() { glBindVertexArray(id); }

  void bind_buffer(GLuint buffer, GLsizei stride, GLuint divisor = 0) const noexcept {
    glBindVertexBuffer(buffer, buffer, 0, stride);
    glVertexBindingDivisor(buffer, divisor);
  }

  void add_attib(GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLuint offset) const noexcept {
    glVertexAttribFormat(index, size, type, normalized, offset);
    glEnableVertexAttribArray(index);
    glVertexAttribBinding(index, buffer);
  }

  void add_iattib(GLuint buffer, GLuint index, GLint size, GLenum type, GLuint offset) const noexcept {
    glVertexAttribIFormat(index, size, type, offset);
    glEnableVertexAttribArray(index);
    glVertexAttribBinding(index, buffer);
  }

  operator GLuint() const noexcept { return id; }
  void destroy() { glDeleteBuffers(1, &id); };

private:
  GLuint id = 0;
};

template <typename T> class UniformBuffer {
  GLuint ubo = 0;
  GLuint binding;

public:
  UniformBuffer(GLuint binding, const T &initial) : binding(binding) {
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(T), &initial, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);
  }

  void set(const T &value) {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &value);
  }

  T *map() {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    return static_cast<T *>(glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_WRITE));
  }
  void unmap() {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
  }

  ~UniformBuffer() { glDeleteBuffers(1, &ubo); }
};
}; // namespace gl
