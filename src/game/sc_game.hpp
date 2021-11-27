#pragma once

#include <GLFW/glfw3.h>

#include <array>
#include <boost/json.hpp>
#include <memory>

#include "sc_assets.hpp"
#include "sc_callbacks.hpp"
#include "sc_config.hpp"
#include "sc_renderer.hpp"

using namespace boost;

namespace sc {
class Game {
 public:
  Game();
  void Run();
  void Update();

  json::stream_parser json_parser_;
  std::unique_ptr<Config> const cfg_;
  std::unique_ptr<Assets> const assets_;
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
