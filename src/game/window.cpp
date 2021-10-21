#include "window.hpp"

#include <GLFW/glfw3.h>

#include "../utils/constants.hpp"
#include "callbacks.hpp"
#include "game.hpp"

Window::Window(void *user_ptr, std::uint32_t const initial_width,
               std::uint32_t const initial_height) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  window_ptr_ =
      std::unique_ptr<GLFWwindow, WindowPtrDestroyer>{glfwCreateWindow(
          initial_width, initial_height, Constants::GetFullProjectName().data(),
          nullptr, nullptr)};
  initial_width_ = initial_width;
  initial_height_ = initial_height;

  glfwSetKeyCallback(window_ptr_.get(), keyCallBack);
  glfwSetCursorPosCallback(window_ptr_.get(), cursorPositionCallBack);
  glfwSetScrollCallback(window_ptr_.get(), scrollCallBack);

  glfwSetWindowUserPointer(window_ptr_.get(), user_ptr);
  glfwMakeContextCurrent(window_ptr_.get());
}

bool Window::ShouldClose() const {
  return glfwWindowShouldClose(window_ptr_.get());
}

void Window::Close() const {
  glfwSetWindowShouldClose(window_ptr_.get(), GLFW_TRUE);
}

vk::SurfaceKHR Window::CreateSurface(VkInstance instance) const {
  VkSurfaceKHR surface{};
  if (glfwCreateWindowSurface(instance, window_ptr_.get(), nullptr, &surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Could not create surface!");
  };
  return {surface};
}