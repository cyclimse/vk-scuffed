#pragma once

#include <GLFW/glfw3.h>  // for GLFWwindow, GLFW_KEY_LAST

#include <array>                           // for array
#include <boost/config/detail/suffix.hpp>  // for boost
#include <boost/json.hpp>
#include <boost/json/stream_parser.hpp>  // for stream_parser
#include <memory>                        // for unique_ptr

#include "sc_assets.hpp"  // for Assets
#include "sc_callbacks.hpp"
#include "sc_config.hpp"    // for Config
#include "sc_renderer.hpp"  // for Renderer

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
