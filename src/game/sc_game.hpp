#pragma once

#include <GLFW/glfw3.h>

#include <array>
#include <memory>

#include "sc_callbacks.hpp"
#include "sc_config.hpp"
#include "sc_renderer.hpp"

namespace sc {
class Game {
 public:
  Game();
  void Run();
  void Update();

  std::shared_ptr<Config> const cfg_;
  Renderer renderer_;

 private:
  std::array<bool, GLFW_KEY_LAST> keys_pressed_;

 public:
  friend void keyCallBack(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  friend void cursorPositionCallBack(GLFWwindow *window, double xpos,
                                     double ypos);
  friend void scrollCallBack(GLFWwindow *window, double xpos, double ypos);
};
}  // namespace sc
