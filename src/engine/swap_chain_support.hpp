#ifndef SWAPCHAINSUPPORT_HPP
#define SWAPCHAINSUPPORT_HPP

#include <vulkan/vulkan.hpp>

class SwapChainSupport {
 public:
  SwapChainSupport() = default;

  SwapChainSupport(const vk::PhysicalDevice& device,
                   const vk::SurfaceKHR& surface,
                   const vk::Extent2D& actual_extent);

  bool HasRequiredSupport() {
    return !(formats_.empty() || present_modes_.empty());
  };

  vk::SurfaceFormatKHR ChooseSwapSurfaceFormat() const;
  vk::PresentModeKHR ChooseSwapPresentMode() const;
  vk::Extent2D ChooseSwapExtent() const;

  vk::SurfaceCapabilitiesKHR capabilities_;
  std::vector<vk::SurfaceFormatKHR> formats_;
  std::vector<vk::PresentModeKHR> present_modes_;

 private:
  vk::Extent2D actual_extent_;
};

#endif  // SWAPCHAINSUPPORT_HPP