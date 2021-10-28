#ifndef VULKANENGINE_HPP
#define VULKANENGINE_HPP

#include "../game/window.hpp"
#include "../utils/include_vulkan.hpp"

class VulkanEngine {
 public:
  VulkanEngine(Window const *window_ptr);

 private:
  void createInstance();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();

  Window const *window_ptr_;

  vk::UniqueInstance instance_;
  vk::DispatchLoaderDynamic dldi_;
  vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>
      messenger_;
  vk::UniqueSurfaceKHR surface_;
  vk::PhysicalDevice physical_device_;
  vk::UniqueDevice device_;
  vk::Queue graphics_queue_;
  vk::Queue presentation_queue_;
};

#endif  // VULKANENGINE_HPP
