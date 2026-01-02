#include <component/physics.hpp>
#include <component/rect.hpp>
#include <component/string.hpp>
#include <entt/entt.hpp>
#include <game.hpp>
#include <key.hpp>
#include <renderer/rect.hpp>
#include <renderer/string.hpp>
#include <systems/ball.hpp>
#include <systems/paddle.hpp>
#include <systems/physics.hpp>
#include <systems/score.hpp>

static entt::registry registry;
static entt::entity ball, score, win_text;
static entt::entity paddles[2];
static bool paused = true;

void init() {
  ball = registry.create();
  registry.emplace<RectComp>(ball, (SPACE - vec2(BALL_SIZE)) / 2.0f, u8vec2(BALL_SIZE));
  registry.emplace<Velocity>(ball, 0, 0);

  for (auto &entity : paddles) {
    entity = registry.create();

    registry.emplace<Velocity>(entity, 0, 0);
    registry.emplace<paddle::Collision>(entity, 0);
  }

  registry.emplace<RectComp>(paddles[0], PADDLE_POS, PADDLE_SIZE);
  registry.emplace<RectComp>(paddles[1], vec2(SPACE.x - (PADDLE_POS.x + PADDLE_SIZE.x), PADDLE_POS.y), PADDLE_SIZE);

  for (int i = 1; i < 28; i += 2) {
    const auto entity = registry.create();
    const u8vec2 size = {1, 3};
    registry.emplace<RectComp>(entity, vec2((SPACE.x - size.x) / 2, i * size.y), size);
  }

  constexpr u8 score_size = 12;
  constexpr float score_padding = 2;

  score = registry.create();
  registry.emplace<StringComp>(score, vec2((SPACE.x - (score_size * 4)) / 2, SPACE.y - (score_size + score_padding)), score_size, "0  0");

  constexpr u8 win_size = 15;
  constexpr float win_padding = 20;

  win_text = registry.create();
  registry.emplace<StringComp>(win_text, vec2((SPACE.x - (score_size * 8.5f)) / 2, SPACE.y - (win_size + win_padding)), win_size, "");

  renderer::rect::init();
  renderer::string::init();
}

void render(float delta_t) {
  update_position(registry, delta_t);

  renderer::rect::render(registry);
  renderer::string::render(registry);
}

void update() {
  if (key::get_event(key::Event::Press, GLFW_KEY_ENTER) && paused) {
    set_ball_initial_vel(registry, ball);
    registry.get<StringComp>(win_text).count = 0;
    paused = false;
  }

  handle_paddle_input(registry, paddles);
  handle_paddle_collision(registry, paddles);
  handle_ball_wall_collision(registry, ball);
  handle_ball_paddle_collision(registry, ball, paddles);
  paused |= handle_score(registry, score, ball, win_text);
}

void clean() {
  registry.clear();
  renderer::rect::clean();
  renderer::string::clean();
}
