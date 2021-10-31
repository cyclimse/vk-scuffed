#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <string_view>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

struct WindowPtrDestroyer {
  void operator()(GLFWwindow *window_ptr) { glfwDestroyWindow(window_ptr); }
};

class Window {
 public:
  Window(void *user_ptr, std::uint32_t const initial_width = 800u,
         std::uint32_t const initial_height = 600u);
  bool ShouldClose() const;
  void Close() const;

  // Vulkan related
  vk::SurfaceKHR CreateSurface(VkInstance instance) const;
  vk::Extent2D GetExtent() const;

 private:
  std::unique_ptr<GLFWwindow, WindowPtrDestroyer> window_ptr_;
  std::uint32_t initial_width_;
  std::uint32_t initial_height_;
};

#endif  // WINDOW_HPP
