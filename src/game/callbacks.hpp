#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include "../utils/include_glfw.hpp"

void keyCallBack(GLFWwindow *window, int key, int scancode, int action,
                 int mods);

void cursorPositionCallBack(GLFWwindow *window, double xpos, double ypos);

void scrollCallBack(GLFWwindow *window, double xpos, double ypos);

#endif  // CALLBACKS_HPP
