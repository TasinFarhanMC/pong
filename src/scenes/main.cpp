#include <GLFW/glfw3.h>
#include <global.hpp>
#include <key.hpp>
#include <logger.hpp>

#include "scene.hpp"

bool init();
void render();
void update(float delta_time);
void clean();

Scene MainScene {init, render, update, clean};
static Scene *that = &MainScene;

bool init() { return true; }

void render() {};

void update(float delta_time) {
  if (key::get(GLFW_KEY_ESCAPE)) { request_exit(0); }
};

void clean() {};
