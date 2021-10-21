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

  Window const *window_ptr_;

  vk::UniqueInstance instance_;
  vk::DispatchLoaderDynamic dldi_;
  vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>
      messenger_;
  vk::UniqueSurfaceKHR surface_;
};

#endif  // VULKANENGINE_HPP
