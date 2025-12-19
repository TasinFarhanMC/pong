#include <component/string.hpp>
#include <entt/entt.hpp>
#include <graphics/gltypes.hpp>
#include <graphics/shader.hpp>
#include <graphics/texture.hpp>
#include <logger.hpp>

constexpr const char *vert_shader = R"(
#version 330 core
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) in vec2 base_pos;
layout(location = 1) in vec2 pos;
layout(location = 2) in uvec4 data;

const vec2 DIM = vec2(160.0f, 90.0f);

out flat uvec4 fData;
out vec2 uv;

const uint GLYPH_WIDTH   = 3u;
const uint GLYPH_PADDING = 1u;

void main() {
  const uint scale = data.x & 0xffu;
  const uint count = (data.x >> 8u) & 0xffu;

  uv = base_pos;
  uv.x *= count;

  const vec2 world_pos = uv * scale + pos;
  gl_Position = vec4(world_pos * 2 / DIM - 1, 0.0f, 1.0f);
  fData = data;
  uv.x *= float(count * (GLYPH_WIDTH + GLYPH_PADDING) + GLYPH_PADDING) / float((GLYPH_WIDTH + GLYPH_PADDING) * count);
}
)";

constexpr const char *frag_shader = R"(
#version 330 core
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_explicit_uniform_location : enable

in flat uvec4 fData;
in vec2 uv;

layout(binding = 0) uniform sampler2D font;

const uint DATA_OFFSET = 2u;

const uint GLYPH_START   = 32u;
const uint GLYPH_COUNT   = 59u;
const uint GLYPH_WIDTH   = 3u;
const uint GLYPH_PADDING = 1u;

const float FONT_PIXEL_NORM  = 1.0 / float((GLYPH_WIDTH + GLYPH_PADDING) * GLYPH_COUNT + 1u);
const float GLYPH_NORM_WIDTH = float(GLYPH_WIDTH + GLYPH_PADDING) * FONT_PIXEL_NORM;

out vec4 color;

uint getByte(uvec4 fData, uint index) {
    uint comp = index >> 2u;     
    uint byteInComp = index & 3u;
    uint value = fData[comp];
    return (value >> (byteInComp * 8u)) & 0xFFu;
}

void main() {
    float glyph_norm = fract(uv.x) * GLYPH_NORM_WIDTH;
    uint byte = getByte(fData, uint(uv.x) + DATA_OFFSET);
    float index_norm = float((byte - GLYPH_START) * (GLYPH_WIDTH + GLYPH_PADDING)) * FONT_PIXEL_NORM;

    color = texture(font, vec2(index_norm + glyph_norm, uv.y));
}
)";

static gl::Buffer<StringComp> vertex(GL_ARRAY_BUFFER);
static gl::Array<vec2> base_vertex(GL_ARRAY_BUFFER);
static gl::VertexArray vao;
static GLuint shader;
static Texture2D font("assets/fonts/bitmap.png", Texture2D::PIXELATED);

namespace renderer {
namespace string {
  void init() {
    vertex.init();
    base_vertex.init({{1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}});
    vao.init();

    vao.bind();
    vao.bind_buffer(base_vertex, sizeof(vec2));
    vao.add_attib(base_vertex, 0, 2, GL_FLOAT, false, 0);

    vao.bind_buffer(vertex, sizeof(StringComp), 1);
    vao.add_attib(vertex, 1, 2, GL_FLOAT, false, offsetof(StringComp, pos));
    vao.add_iattib(vertex, 2, 4, GL_UNSIGNED_INT, offsetof(StringComp, scale));

    shader = link_program(vert_shader, frag_shader);

    font.init();
    font.bind(0);
  }

  void render(entt::registry &registry) {
    auto &storage = registry.storage<StringComp>();
    vertex.update(*storage.raw(), storage.size());

    vao.bind();
    glUseProgram(shader);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, storage.size());
  }

  void clean() {
    font.destroy();
    vao.destroy();
    vertex.destroy();
    base_vertex.destroy();
    glDeleteProgram(shader);
  }
} // namespace string
} // namespace renderer
