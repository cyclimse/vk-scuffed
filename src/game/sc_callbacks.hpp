#pragma once

#include <GLFW/glfw3.h>

namespace sc {
void keyCallBack(GLFWwindow *window, int key, int scancode, int action,
                 int mods);

void cursorPositionCallBack(GLFWwindow *window, double xpos, double ypos);

void scrollCallBack(GLFWwindow *window, double xpos, double ypos);

}  // namespace sc
