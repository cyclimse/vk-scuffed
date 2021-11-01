#include "sc_callbacks.hpp"

#include "sc_game.hpp"

using namespace sc;

void sc::keyCallBack(GLFWwindow *window, int key, [[maybe_unused]] int scancode,
                 int action, [[maybe_unused]] int mods) {
  if (key != GLFW_KEY_UNKNOWN) {
    Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window));
    game->keys_pressed_[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
  }
}

void sc::cursorPositionCallBack(GLFWwindow *window, [[maybe_unused]] double xpos,
                            [[maybe_unused]] double ypos) {
  [[maybe_unused]] Game *game =
      static_cast<Game *>(glfwGetWindowUserPointer(window));
}

void sc::scrollCallBack(GLFWwindow *window, [[maybe_unused]] double xpos,
                    [[maybe_unused]] double ypos) {
  [[maybe_unused]] Game *game =
      static_cast<Game *>(glfwGetWindowUserPointer(window));
}
