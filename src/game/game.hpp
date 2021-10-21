#ifndef GAME_HPP
#define GAME_HPP

#include <array>

#include "../engine/vulkanengine.hpp"
#include "callbacks.hpp"
#include "window.hpp"

class Game {
 public:
  Game();
  void Run();
  void Update();

 private:
  Window window_;
  VulkanEngine engine_;

  std::array<bool, GLFW_KEY_LAST> keys_pressed_;

 public:
  friend void keyCallBack(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  friend void cursorPositionCallBack(GLFWwindow *window, double xpos,
                                     double ypos);
  friend void scrollCallBack(GLFWwindow *window, double xpos, double ypos);
};

#endif  // GAME_HPP
