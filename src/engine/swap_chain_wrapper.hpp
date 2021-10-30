#ifndef SWAPCHAINWRAPPER_HPP
#define SWAPCHAINWRAPPER_HPP

#include "../utils/include_vulkan.hpp"
#include "queue_family_indices.hpp"
#include "swap_chain_support.hpp"
#include "vk_types.hpp"

struct SwapChainFrame {
  vk::Image image;
  vk::UniqueImageView image_view;
  vk::UniqueFramebuffer frame_buffer;
  AllocatedBuffer uniform_buffer;
  AllocatedBuffer storage_buffer;
  vk::CommandBuffer command_buffer;
};

class SwapChainWrapper {
 public:
  SwapChainWrapper() = default;

  SwapChainWrapper(vk::SurfaceKHR const &surface, vk::Device const &device,
                   SwapChainSupport const &swap_chain_support,
                   QueueFamilyIndices const &indices);

  vk::UniqueSwapchainKHR swap_chain_;
  vk::Format swap_chain_image_format_;
  vk::Extent2D swap_chain_image_extent_;

  std::vector<SwapChainFrame> frames_;

 private:
  // Not managed by this class
  vk::SurfaceKHR surface_;
  vk::Device device_;

  SwapChainSupport swap_chain_support_;
  QueueFamilyIndices indices_;

  void createSwapChain();
};

#endif  // SWAPCHAINSUPPORT_HPP