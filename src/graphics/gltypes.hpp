#pragma once

#include <glad/gl.h>
#include <initializer_list>
#include <logger.hpp>

namespace gl {
template <typename T> class Buffer {
public:
  Buffer(GLenum type) : type(type) {};

  void destroy() {
    if (id) {
      glDeleteBuffers(1, &id);
      LOG_INFO("Graphics/GL/Buffer", "Deleted buffer {}", id);
      id = 0;
    }
  }

  operator GLuint() const noexcept { return this->id; }

  void init(const std::initializer_list<T> data = {}) {
    glGenBuffers(1, &id);
    glBindBuffer(type, id);
    glBufferData(type, data.size() * sizeof(T), data.begin(), GL_DYNAMIC_DRAW);
    LOG_INFO("Graphics/GL/Buffer", "Initialized buffer {} with {} elements", id, data.size());
  }

  T *map() {
    glBindBuffer(type, id);
    LOG_VERB("Graphics/GL/Buffer", "Mapped buffer {}", id);
    return static_cast<T *>(glMapBuffer(type, GL_READ_WRITE));
  }

  void unmap() {
    glBindBuffer(type, id);
    glUnmapBuffer(type);
    LOG_VERB("Graphics/GL/Buffer", "Unmapped buffer {}", id);
  }

  void update(const T *data, int size) {
    glBindBuffer(type, id);
    glBufferData(type, sizeof(T) * size, data, GL_DYNAMIC_DRAW);
    LOG_VERB("Graphics/GL/Buffer", "Updated buffer {} with {} elements", id, size);
  }

private:
  const GLenum type;
  GLuint id = 0;
};

template <typename T> class Array {
public:
  Array(GLenum type) : type(type) {};

  void destroy() {
    if (id) {
      glDeleteBuffers(1, &id);
      LOG_INFO("Graphics/GL/Array", "Deleted array buffer {}", id);
      id = 0;
    }
  }

  operator GLuint() const noexcept { return this->id; }

  void init(const std::initializer_list<T> data) {
    glGenBuffers(1, &id);
    glBindBuffer(type, id);
    glBufferData(type, data.size() * sizeof(T), data.begin(), GL_STATIC_DRAW);
    LOG_INFO("Graphics/GL/Array", "Initialized array buffer {} with {} elements", id, data.size());
  }

  T *map() {
    glBindBuffer(type, id);
    LOG_VERB("Graphics/GL/Array", "Mapped array buffer {}", id);
    return static_cast<T *>(glMapBuffer(type, GL_READ_ONLY));
  }

  void unmap() {
    glBindBuffer(type, id);
    glUnmapBuffer(type);
    LOG_VERB("Graphics/GL/Array", "Unmapped array buffer {}", id);
  }

  Array(Array &&) = delete;
  Array(const Array &) = delete;
  Array &operator=(Array &&) = delete;
  Array &operator=(const Array &) = delete;

private:
  const GLenum type;
  GLuint id = 0;
};

class VertexArray {
public:
  void init() {
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
    LOG_INFO("Graphics/GL/VAO", "Created VertexArray {}", id);
  }

  void bind() { glBindVertexArray(id); }

  void bind_buffer(GLuint buffer, GLsizei stride, GLuint divisor = 0) const noexcept {
    glBindVertexBuffer(buffer, buffer, 0, stride);
    glVertexBindingDivisor(buffer, divisor);
    LOG_VERB("Graphics/GL/VAO", "Bound buffer {} with stride {} and divisor {}", buffer, stride, divisor);
  }

  void add_attib(GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLuint offset) const noexcept {
    glVertexAttribFormat(index, size, type, normalized, offset);
    glEnableVertexAttribArray(index);
    glVertexAttribBinding(index, buffer);
    LOG_VERB("Graphics/GL/VAO", "Added attribute {} to buffer {}", index, buffer);
  }

  void add_iattib(GLuint buffer, GLuint index, GLint size, GLenum type, GLuint offset) const noexcept {
    glVertexAttribIFormat(index, size, type, offset);
    glEnableVertexAttribArray(index);
    glVertexAttribBinding(index, buffer);
    LOG_VERB("Graphics/GL/VAO", "Added integer attribute {} to buffer {}", index, buffer);
  }

  operator GLuint() const noexcept { return id; }

  void destroy() {
    if (id) {
      glDeleteVertexArrays(1, &id);
      LOG_INFO("Graphics/GL/VAO", "Deleted VertexArray {}", id);
      id = 0;
    }
  }

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
    LOG_INFO("Graphics/GL/UBO", "Created UniformBuffer {} bound to slot {}", ubo, binding);
  }

  void set(const T &value) {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &value);
    LOG_VERB("Graphics/GL/UBO", "Updated UniformBuffer {}", ubo);
  }

  T *map() {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    LOG_VERB("Graphics/GL/UBO", "Mapped UniformBuffer {}", ubo);
    return static_cast<T *>(glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_WRITE));
  }

  void unmap() {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    LOG_VERB("Graphics/GL/UBO", "Unmapped UniformBuffer {}", ubo);
  }

  ~UniformBuffer() {
    if (ubo) {
      glDeleteBuffers(1, &ubo);
      LOG_INFO("Graphics/GL/UBO", "Deleted UniformBuffer {}", ubo);
      ubo = 0;
    }
  }
};

} // namespace gl
