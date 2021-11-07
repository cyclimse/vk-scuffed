#include "sc_window.hpp"

#include <GLFW/glfw3.h>

#include "sc_callbacks.hpp"

using namespace sc;

Window::Window(vk::Extent2D resolution, std::string_view name, void *user_ptr) {
  window_ptr_ =
      std::unique_ptr<GLFWwindow, WindowPtrDestroyer>{glfwCreateWindow(
          resolution.width, resolution.height, name.data(), nullptr, nullptr)};

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

vk::Extent2D Window::GetExtent() const {
  int width, height;
  glfwGetFramebufferSize(window_ptr_.get(), &width, &height);
  return {static_cast<std::uint32_t>(width),
          static_cast<std::uint32_t>(height)};
}
