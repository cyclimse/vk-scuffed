#ifndef VULKANENGINE_HPP
#define VULKANENGINE_HPP

#include "../utils/include_vulkan.hpp"

class VulkanEngine {
 public:
  VulkanEngine();

 private:
  void createInstance();

  vk::UniqueInstance instance_;
  vk::DispatchLoaderDynamic dldi_;
  vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>
      messenger_;

#ifndef NDEBUG

#else
  static constexpr bool kEnableValidationLayer = false;
#endif
};

#endif  // VULKANENGINE_HPP
