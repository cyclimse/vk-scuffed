#ifndef SWAPCHAINSUPPORT_HPP
#define SWAPCHAINSUPPORT_HPP

#include "../utils/include_vulkan.hpp"

class SwapChainSupport {
 public:
  SwapChainSupport(const vk::PhysicalDevice& device,
                   const vk::SurfaceKHR& surface);

  bool HasRequiredSupport() {
    return !(formats_.empty() || present_modes_.empty());
  };

  vk::SurfaceFormatKHR ChooseSwapSurfaceFormat() const;
  vk::PresentModeKHR ChooseSwapPresentMode() const;
  vk::Extent2D ChooseSwapExtent(vk::Extent2D actual_extent) const;

 private:
  vk::SurfaceCapabilitiesKHR capabilities_;
  std::vector<vk::SurfaceFormatKHR> formats_;
  std::vector<vk::PresentModeKHR> present_modes_;
};

#endif  // SWAPCHAINSUPPORT_HPP