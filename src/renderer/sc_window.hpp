#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <memory>
#include <string_view>
#include <vulkan/vulkan.hpp>

namespace sc {
class Window {
 public:
  Window(vk::Extent2D const resolution, std::string_view const name,
         void *user_ptr);

  bool ShouldClose() const;
  void Close() const;

  // Vulkan related
  vk::SurfaceKHR CreateSurface(VkInstance instance) const;
  vk::Extent2D GetExtent() const;

 private:
  struct WindowPtrDestroyer {
    void operator()(GLFWwindow *window_ptr) { glfwDestroyWindow(window_ptr); }
  };
  std::unique_ptr<GLFWwindow, WindowPtrDestroyer> window_ptr_;
};
}  // namespace sc
