#pragma once

#include <glad/gl.h>
#include <logger.hpp>
#include <stb_image.h>
#include <string>

class Texture2D {
public:
  // Texture flags
  enum Flags : unsigned int { NONE = 0, PIXELATED = 1 << 0, INTERPOLATED = 1 << 1, CLAMP = 1 << 2, REPEAT = 1 << 3 };

  Texture2D(const std::string &path = "", unsigned int flags = NONE) : path(path), flags(flags) {};

  void destroy() {
    if (id) {
      glDeleteTextures(1, &id);
      LOG_INFO("Graphics/Texture", "Destroyed texture: {}", path);
      id = 0;
    }
  }

  operator GLuint() const noexcept { return id; }

  void init(bool flipY = false) { init(path, flipY); }

  void init(const std::string &path, bool flipY = false) {
    this->path = path;
    if (!flipY) stbi_set_flip_vertically_on_load(1);

    int desiredChannels = 0; // 0 = load as-is
    unsigned char *pixels = stbi_load(path.c_str(), &width, &height, &channels, desiredChannels);
    if (!pixels) {
      LOG_ERROR("Graphics/Texture", "Failed to load texture: {}", path);
      return;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    GLenum internalFormat, format;

    switch (channels) {
    case 1:
      internalFormat = GL_R8;
      format = GL_RED;
      break;
    case 3:
      internalFormat = GL_RGB8;
      format = GL_RGB;
      break;
    case 4:
      internalFormat = GL_RGBA8;
      format = GL_RGBA;
      break;
    default:
      LOG_ERROR("Graphics/Texture", "Unsupported channel count: {} in {}", channels, path);
      stbi_image_free(pixels);
      return;
    }

    // Immutable storage
    glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);

    // Upload pixels
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Texture filtering
    if (flags & PIXELATED) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    } else if (flags & INTERPOLATED) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
      // Default
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    // Texture wrapping
    GLenum wrapMode = (flags & CLAMP) ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    if (flags & REPEAT) wrapMode = GL_REPEAT;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    // Conditional swizzle for grayscale
    if (channels == 1) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
      LOG_INFO("Graphics/Texture", "Applied grayscale swizzle for {}", path);
    }

    stbi_image_free(pixels);
    LOG_INFO("Graphics/Texture", "Loaded texture {} ({}x{}, {} channels)", path, width, height, channels);
  }

  void bind(GLuint slot = 0) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
  }

private:
  std::string path;
  GLuint id = 0;
  int width = 0, height = 0, channels = 0;
  unsigned int flags = NONE;
};
