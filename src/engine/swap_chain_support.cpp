#include "swap_chain_support.hpp"

#include <vulkan/vulkan_structs.hpp>

SwapChainSupport::SwapChainSupport(const vk::PhysicalDevice& device,
                                   const vk::SurfaceKHR& surface,
                                   const vk::Extent2D& actual_extent)
    : capabilities_{device.getSurfaceCapabilitiesKHR(surface)},
      formats_{device.getSurfaceFormatsKHR(surface)},
      present_modes_{device.getSurfacePresentModesKHR(surface)},
      actual_extent_{actual_extent} {}

vk::SurfaceFormatKHR SwapChainSupport::ChooseSwapSurfaceFormat() const {
  auto const result = std::find_if(
      formats_.cbegin(), formats_.cend(),
      [](vk::SurfaceFormatKHR const& available_format) {
        return available_format.format == vk::Format::eB8G8R8A8Srgb &&
               available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
      });
  if (result != formats_.cend()) {
    return *result;
  } else {
    return formats_.front();
  }
}

vk::PresentModeKHR SwapChainSupport::ChooseSwapPresentMode() const {
  auto const result = std::find_if(
      present_modes_.cbegin(), present_modes_.cend(),
      [](vk::PresentModeKHR const& available_present_modes) {
        // Reference material:
        // https://software.intel.com/content/www/us/en/develop/articles/api-without-secrets-introduction-to-vulkan-part-2.html
        return available_present_modes == vk::PresentModeKHR::eMailbox;
      });
  if (result != present_modes_.cend()) {
    return *result;
  } else {
    return present_modes_.front();
  }
}

vk::Extent2D SwapChainSupport::ChooseSwapExtent() const {
  if (capabilities_.currentExtent.width != UINT32_MAX) {
    // Window managers will set the currentExtent to UINT32_MAX if they allow
    // us to change the resolution of the swap chain images from the resolution
    // of the windows.
    return capabilities_.currentExtent;
  } else {
    return {
        std::clamp(actual_extent_.width, capabilities_.minImageExtent.width,
                   capabilities_.maxImageExtent.width),
        std::clamp(actual_extent_.height, capabilities_.maxImageExtent.width,
                   capabilities_.maxImageExtent.width)};
  }
}
