#pragma once
#include <glad/gl.h>
#include <logger.hpp>

inline GLuint compile_shader(GLenum type, const char *src) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLint lenght;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &lenght);
    auto log = new char[lenght];

    glGetShaderInfoLog(shader, lenght, nullptr, log);

    LOG_ERROR("Graphics/Shader", "Shader Compilation Failed {}: {{\n{}\n}}", shader, log);

    delete[] log;
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

inline GLuint link_program(const char *vertex, const char *fragment) {
  GLuint program = glCreateProgram();
  GLuint vert_shader = compile_shader(GL_VERTEX_SHADER, vertex);
  GLuint frag_shader = compile_shader(GL_FRAGMENT_SHADER, fragment);

  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);
  glLinkProgram(program);

  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    auto log = new char[length];

    glGetProgramInfoLog(program, length, nullptr, log);

    LOG_ERROR("Graphics/Shader", "Shader Program Linking Failed: {}\n{}", program, log);

    delete[] log;
    glDeleteProgram(program);
    program = 0;
  }

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
  return program;
}
