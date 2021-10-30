#include "swap_chain_wrapper.hpp"

SwapChainWrapper::SwapChainWrapper(vk::SurfaceKHR const &surface,
                                   vk::Device const &device,
                                   SwapChainSupport const &swap_chain_support,
                                   QueueFamilyIndices const &indices)
    : surface_{surface},
      device_{device},
      swap_chain_support_{swap_chain_support},
      indices_{indices} {}

void SwapChainWrapper::createSwapChain() {
  vk::SurfaceFormatKHR const surface_format =
      swap_chain_support_.ChooseSwapSurfaceFormat();
  vk::PresentModeKHR const present_mode =
      swap_chain_support_.ChooseSwapPresentMode();
  vk::Extent2D const extent = swap_chain_support_.ChooseSwapExtent();

  // It is recommended to ask for one more image that minimum such that we
  // don't have to wait for the driver to complete internal operations.
  std::uint32_t image_count =
      swap_chain_support_.capabilities_.minImageCount + 1;

  // If the maximum is set to 0, it means that there is no maximum. Hence why
  // we must exclude it.
  if (swap_chain_support_.capabilities_.maxImageCount > 0 &&
      image_count > swap_chain_support_.capabilities_.maxImageCount) {
    image_count = swap_chain_support_.capabilities_.maxImageCount;
  }

  vk::SwapchainCreateInfoKHR create_info{
      .surface = surface_,
      .minImageCount = image_count,
      .imageFormat = surface_format.format,
      .imageColorSpace = surface_format.colorSpace,
      .imageExtent = extent,
      .imageArrayLayers = 1,
      .imageUsage = vk::ImageUsageFlagBits::eColorAttachment};

  std::uint32_t queue_family_indices[] = {indices_.graphics_family.value(),
                                          indices_.present_family.value()};

  if (indices_.graphics_family != indices_.present_family) {
    create_info.imageSharingMode = vk::SharingMode::eConcurrent;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices = queue_family_indices;
  } else {
    create_info.imageSharingMode = vk::SharingMode::eExclusive;
  }

  create_info.preTransform = swap_chain_support_.capabilities_.currentTransform;
  create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;

  swap_chain_ = device_.createSwapchainKHRUnique(create_info);

  auto const swap_chain_images =
      device_.getSwapchainImagesKHR(swap_chain_.get());

  frames_.resize(swap_chain_images.size());
  for (auto i = 0u; i < swap_chain_images.size(); i++) {
    frames_[i].image = swap_chain_images[i];
  }

  swap_chain_image_format_ = surface_format.format;
  swap_chain_image_extent_ = extent;
}