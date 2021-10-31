#ifndef VULKANENGINE_HPP
#define VULKANENGINE_HPP

#include "../game/window.hpp"
#include <vulkan/vulkan.hpp>

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

  vk::UniqueHandle<vk::Instance, vk::DispatchLoaderDynamic> instance_;
  vk::DispatchLoaderDynamic dldi_;
  vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>
      messenger_;
  vk::UniqueHandle<vk::SurfaceKHR, vk::DispatchLoaderDynamic> surface_;
  vk::PhysicalDevice physical_device_;
  vk::UniqueHandle<vk::Device, vk::DispatchLoaderDynamic> device_;
  vk::Queue graphics_queue_;
  vk::Queue presentation_queue_;
  std::unique_ptr<VmaAllocator_T, VmaAllocatorPtrDestroyer> allocator_;
  vk::UniqueHandle<vk::SwapchainKHR, vk::DispatchLoaderDynamic> swap_chain_;
  // std::vector<vk::UniqueHandle<vk::Image, vk::DynamicLoader>> images_;
};

#endif  // VULKANENGINE_HPP
