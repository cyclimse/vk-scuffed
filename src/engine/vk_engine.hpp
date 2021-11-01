#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

#include "sc_config.hpp"
#include "sc_window.hpp"
#include "vk_types.hpp"

class VulkanEngine {
 public:
  VulkanEngine(std::shared_ptr<sc::Config> const cfg,
               sc::Window const *window_ptr);

 private:
  void createInstance();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createVmaAllocator();
  void createSwapChain();
  void createRenderPass();

  std::shared_ptr<sc::Config> const cfg_;
  sc::Window const *window_ptr_;

  // Vulkan setup
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

  // Swap chain
  vk::UniqueHandle<vk::SwapchainKHR, vk::DispatchLoaderDynamic> swap_chain_;
  std::vector<SwapChainFrame> swap_chain_frames_;
  vk::Extent2D swap_chain_image_extent_;
  vk::Format swap_chain_image_format_;
};