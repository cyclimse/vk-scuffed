#pragma once

#include <memory>             // for unique_ptr
#include <vector>             // for vector
#include <vulkan/vulkan.hpp>  // for DispatchLoaderDynamic, UniqueHandle
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

#include "pipeline_factory.hpp"  // for PipelineFactory
#include "sc_assets.hpp"
#include "sc_config.hpp"
#include "sc_material.hpp"
#include "sc_window.hpp"
#include "vk_types.hpp"  // for SwapChainFrame, VmaAllocatorPtrDestr...

namespace sc {
class Window;
struct Config;
}  // namespace sc
struct VmaAllocator_T;

class VulkanEngine {
 public:
  VulkanEngine(sc::Config const *cfg, sc::Assets const *assets,
               sc::Window const *window_ptr);
#ifdef __SANITIZE_ADDRESS__
  // This looks terrible but seem to be necessary to avoid warning pollution
  // from ASAN with NVIDIA proprietary drivers
  inline ~VulkanEngine() { instance_.release(); }
#endif
 private:
  void createInstance();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createVmaAllocator();
  void createSwapChain();
  void createImageViews();
  void createRenderPass();
  void createDescriptorSetLayout();
  void createResources();
  void createPipelines();

  sc::Config const *cfg_;
  sc::Assets const *assets_;
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

  // Render pass
  vk::UniqueHandle<vk::RenderPass, vk::DispatchLoaderDynamic> render_pass_;

  // Layout descriptors
  vk::UniqueHandle<vk::DescriptorSetLayout, vk::DispatchLoaderDynamic>
      uniform_descriptor_set_layout_;
  vk::UniqueHandle<vk::DescriptorSetLayout, vk::DispatchLoaderDynamic>
      global_descriptor_set_layout_;
  vk::UniqueHandle<vk::PipelineLayout, vk::DispatchLoaderDynamic>
      pipeline_layout_;

  // Pipeline
  PipelineFactory pipeline_factory_;

  std::vector<sc::Shader> shaders_;
  std::vector<sc::Material> materials_;
};
