#include "callbacks.hpp"

#include "game.hpp"

void keyCallBack(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action,
                 [[maybe_unused]] int mods) {
  if (key != GLFW_KEY_UNKNOWN) {
    Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window));
    game->keys_pressed_[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
  }
}

void cursorPositionCallBack(GLFWwindow *window, double xpos, double ypos) {
  Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window));
}

void scrollCallBack(GLFWwindow *window, double xpos, double ypos) {
  Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window));
}
