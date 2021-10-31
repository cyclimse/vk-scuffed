#include "vulkan_engine.hpp"

#include <cstdlib>
#include <set>
#include <vector>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

#include "../utils/constants.hpp"
#include "queue_family_indices.hpp"
#include "vk_utils.hpp"

VulkanEngine::VulkanEngine(Window const *window_ptr) : window_ptr_(window_ptr) {
  createInstance();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createVmaAllocator();
  createSwapChain();
}

void VulkanEngine::createInstance() {
  auto glfw_extension_count = 0u;
  auto const glfw_extensions =
      glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  std::vector<const char *> extensions{glfw_extensions,
                                       glfw_extensions + glfw_extension_count};
  std::vector<const char *> layers{};

  if constexpr (Constants::Engine::kEnableValidationLayer) {
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    layers.push_back("VK_LAYER_KHRONOS_validation");
  }

  vk::ApplicationInfo application_info{
      .pApplicationName = Constants::kProjectName.data(),
      .applicationVersion = Constants::kVersion,
      .pEngineName = Constants::kEngineName.data(),
      .engineVersion = Constants::kVersion,
      .apiVersion = VK_API_VERSION_1_1};

  vk::InstanceCreateInfo instance_create_info{
      .pApplicationInfo = &application_info,
      .enabledLayerCount = static_cast<std::uint32_t>(layers.size()),
      .ppEnabledLayerNames = layers.data(),
      .enabledExtensionCount = static_cast<std::uint32_t>(extensions.size()),
      .ppEnabledExtensionNames = extensions.data()};

  dldi_ = vk::DispatchLoaderDynamic(vkGetInstanceProcAddr);

  if constexpr (Constants::Engine::kEnableValidationLayer) {
    vk::StructureChain<vk::InstanceCreateInfo,
                       vk::DebugUtilsMessengerCreateInfoEXT>
        chain = {instance_create_info,
                 GetDebugUtilsMessengerCreateInfoStruct()};
    instance_ = vk::createInstanceUnique(chain.get<vk::InstanceCreateInfo>(),
                                         nullptr, dldi_);
  } else {
    instance_ = vk::createInstanceUnique(instance_create_info, nullptr, dldi_);
  }

  dldi_.init(instance_.get());

  if constexpr (Constants::Engine::kEnableValidationLayer) {
    messenger_ = instance_->createDebugUtilsMessengerEXTUnique(
        GetDebugUtilsMessengerCreateInfoStruct(), nullptr, dldi_);
  }
}

void VulkanEngine::createSurface() {
  vk::ObjectDestroy<vk::Instance, vk::DispatchLoaderDynamic> _deleter(
      instance_.get(), nullptr, dldi_);
  surface_ = vk::UniqueHandle<vk::SurfaceKHR, vk::DispatchLoaderDynamic>{
      window_ptr_->CreateSurface(instance_.get()), _deleter};
}

void VulkanEngine::pickPhysicalDevice() {
  auto const devices = instance_->enumeratePhysicalDevices();

  for (const auto &device : devices) {
    if (IsDeviceSuitable(device, surface_.get())) {
      physical_device_ = device;
#ifndef NDEBUG
      std::clog << "Chose physical device: "
                << physical_device_.getProperties().deviceName << std::endl;
#endif
      return;
    }
  }

  throw std::runtime_error("Could not find a suitable GPU");
}

void VulkanEngine::createLogicalDevice() {
  QueueFamilyIndices indices{physical_device_, surface_.get()};

  std::vector<vk::DeviceQueueCreateInfo> queue_create_infos{};
  std::set<std::uint32_t> unique_queue_families = {
      indices.graphics_family.value(), indices.present_family.value()};

  float queue_priority = 1.0;
  for (auto queue_family : unique_queue_families) {
    queue_create_infos.push_back(
        vk::DeviceQueueCreateInfo{.queueFamilyIndex = queue_family,
                                  .queueCount = 1,
                                  .pQueuePriorities = &queue_priority});
  }

  vk::PhysicalDeviceFeatures features{};

  vk::DeviceCreateInfo create_info{
      .queueCreateInfoCount =
          static_cast<std::uint32_t>(queue_create_infos.size()),
      .pQueueCreateInfos = queue_create_infos.data(),
      .enabledExtensionCount = static_cast<std::uint32_t>(
          Constants::Engine::kDeviceExtensions.size()),
      .ppEnabledExtensionNames = Constants::Engine::kDeviceExtensions.data(),
      .pEnabledFeatures = &features};

  device_ = physical_device_.createDeviceUnique(create_info, nullptr, dldi_);
  dldi_.init(device_.get());

  graphics_queue_ = device_->getQueue(indices.graphics_family.value(), 0);
  presentation_queue_ = device_->getQueue(indices.present_family.value(), 0);
}

void VulkanEngine::createVmaAllocator() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
  VmaAllocatorCreateInfo allocator_info{
      .flags = {},
      .physicalDevice = physical_device_,
      .device = device_.get(),
      .instance = instance_.get(),
      .vulkanApiVersion = VK_API_VERSION_1_1,
  };
#pragma GCC diagnostic pop

  VmaAllocator allocator;
  vmaCreateAllocator(&allocator_info, &allocator);
  allocator_.reset(allocator);
}

void VulkanEngine::createSwapChain() {
  // wrapped_swap_chain_ = SwapChainWrapper{
  //     surface_.get(),
  //     device_.get(),
  //     {physical_device_, surface_.get(), window_ptr_->GetExtent()},
  //     {physical_device_, surface_.get()}};

  SwapChainSupport swap_chain_support_ = {physical_device_, surface_.get(),
                                          window_ptr_->GetExtent()};
  QueueFamilyIndices indices_ = {physical_device_, surface_.get()};

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
      .surface = surface_.get(),
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

  swap_chain_ = device_->createSwapchainKHRUnique(create_info, nullptr, dldi_);

  // auto const swap_chain_images =
  //     device_->getSwapchainImagesKHR(swap_chain_.get(), dldid_);

  // images_.resize(swap_chain_images.size());
  // for (auto i = 0u; i < swap_chain_images.size(); i++) {
  //   images_[i] = vk::UniqueHandle<vk::Image, vk::DispatchLoaderDynamic>{
  //       swap_chain_images[i]};
  // }

  // swap_chain_image_format_ = surface_format.format;
  // swap_chain_image_extent_ = extent;
}
