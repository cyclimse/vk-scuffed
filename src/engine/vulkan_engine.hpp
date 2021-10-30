#ifndef VULKANENGINE_HPP
#define VULKANENGINE_HPP

#include "../game/window.hpp"
#include "../utils/include_vulkan.hpp"
#include "swap_chain_wrapper.hpp"
#include "vk_types.hpp"

class VulkanEngine {
 public:
  VulkanEngine(Window const *window_ptr);

 private:
  void createInstance();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createVmaAllocator();
  void createSwapChain();

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
  std::unique_ptr<VmaAllocator_T, VmaAllocatorPtrDestroyer> allocator_;
  SwapChainWrapper wrapped_swap_chain_;
};

#endif  // VULKANENGINE_HPP
