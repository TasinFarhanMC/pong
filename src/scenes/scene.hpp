#pragma once

#include <betr/functional.hpp>
#include <betr/unordered_set.hpp>

struct Scene {
  betr::Function<bool(void)> init_func;
  betr::Function<void()> render;
  betr::Function<void(float)> update;
  void (*clean_func)();

  Scene(betr::Function<bool(void)> init, betr::Function<void()> render, betr::Function<void(float)> update, void (*clean)())
      : init_func(init),
        render(render),
        update(update),
        clean_func(clean) {}

  float z;
  Scene *parent;
  Scene **node;
  bool inited = false;
  static betr::UnorderedSet<void (*)()> clean_functions;

  bool init() {
    if (inited) { return true; }

    inited = init_func();
    if (inited) { clean_functions.insert(clean_func); }
    return inited;
  }

  void clean() {
    if (!inited) { return; }

    clean_functions.erase(clean_func);
    clean_func();
  }

  void set(Scene **node, float z = 0.0f) {
    this->node = node;
    this->z = z;
    parent = *node;
    *node = this;
  }

  void set_parent(Scene **node) { *node = parent; }
  void set_parent() { *node = parent; }

  static void clean_all() {
    for (const auto clean : clean_functions) { clean(); }
  }
};
