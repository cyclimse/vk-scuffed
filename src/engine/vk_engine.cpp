#include "vk_engine.hpp"

#include <GLFW/glfw3.h>

#include <cstdlib>
#include <set>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

#include "queue_family_indices.hpp"
#include "sc_config.hpp"
#include "swap_chain_support.hpp"
#include "vk_utils.hpp"

VulkanEngine::VulkanEngine(std::shared_ptr<sc::Config> const cfg,
                           sc::Window const *window_ptr)
    : cfg_{cfg}, window_ptr_(window_ptr) {
  createInstance();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createVmaAllocator();
  createSwapChain();
  createImageViews();
  createRenderPass();
  createDescriptorSetLayout();
  createPipelineFactory();
}

void VulkanEngine::createInstance() {
  auto glfw_extension_count = 0u;
  auto const glfw_extensions =
      glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  std::vector<const char *> extensions{glfw_extensions,
                                       glfw_extensions + glfw_extension_count};
  std::vector<const char *> layers{};

  if (cfg_->eng.validation_layers_enabled) {
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    layers.push_back("VK_LAYER_KHRONOS_validation");
  }

  vk::ApplicationInfo application_info{
      .pApplicationName = cfg_->project_name.c_str(),
      .applicationVersion = cfg_->version,
      .pEngineName = cfg_->eng.engine_name.c_str(),
      .engineVersion = cfg_->version,
      .apiVersion = VK_API_VERSION_1_2};

  vk::InstanceCreateInfo instance_create_info{
      .pApplicationInfo = &application_info,
      .enabledLayerCount = static_cast<std::uint32_t>(layers.size()),
      .ppEnabledLayerNames = layers.data(),
      .enabledExtensionCount = static_cast<std::uint32_t>(extensions.size()),
      .ppEnabledExtensionNames = extensions.data()};

  dldi_ = vk::DispatchLoaderDynamic(vkGetInstanceProcAddr);

  if (cfg_->eng.validation_layers_enabled) {
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

  if (cfg_->eng.validation_layers_enabled) {
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
      if (cfg_->is_debug) {
        std::clog << "Chose physical device: "
                  << physical_device_.getProperties().deviceName << std::endl;
      }
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

  std::vector<const char *> device_extensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  vk::DeviceCreateInfo create_info{
      .queueCreateInfoCount =
          static_cast<std::uint32_t>(queue_create_infos.size()),
      .pQueueCreateInfos = queue_create_infos.data(),
      .enabledExtensionCount =
          static_cast<std::uint32_t>(device_extensions.size()),
      .ppEnabledExtensionNames = device_extensions.data(),
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
      .vulkanApiVersion = VK_API_VERSION_1_2,
  };
#pragma GCC diagnostic pop

  VmaAllocator allocator;
  vmaCreateAllocator(&allocator_info, &allocator);
  allocator_.reset(allocator);
}

void VulkanEngine::createSwapChain() {
  SwapChainSupport swap_chain_support = {physical_device_, surface_.get(),
                                         window_ptr_->GetExtent()};
  QueueFamilyIndices indices = {physical_device_, surface_.get()};

  vk::SurfaceFormatKHR const surface_format =
      swap_chain_support.ChooseSwapSurfaceFormat();
  vk::PresentModeKHR const present_mode =
      swap_chain_support.ChooseSwapPresentMode();
  vk::Extent2D const extent = swap_chain_support.ChooseSwapExtent();

  // It is recommended to ask for one more image that minimum such that we
  // don't have to wait for the driver to complete internal operations.
  std::uint32_t image_count =
      swap_chain_support.capabilities_.minImageCount + 1;

  // If the maximum is set to 0, it means that there is no maximum. Hence why
  // we must exclude it.
  if (swap_chain_support.capabilities_.maxImageCount > 0 &&
      image_count > swap_chain_support.capabilities_.maxImageCount) {
    image_count = swap_chain_support.capabilities_.maxImageCount;
  }

  std::uint32_t queue_family_indices[] = {indices.graphics_family.value(),
                                          indices.present_family.value()};

  vk::SwapchainCreateInfoKHR create_info{
      .surface = surface_.get(),
      .minImageCount = image_count,
      .imageFormat = surface_format.format,
      .imageColorSpace = surface_format.colorSpace,
      .imageExtent = extent,
      .imageArrayLayers = 1,
      .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
      .imageSharingMode = indices.graphics_family != indices.present_family
                              ? vk::SharingMode::eConcurrent
                              : vk::SharingMode::eExclusive,
      .queueFamilyIndexCount =
          indices.graphics_family != indices.present_family ? 2u : 1u,
      .pQueueFamilyIndices = queue_family_indices,
      .preTransform = swap_chain_support.capabilities_.currentTransform,
      .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
      .presentMode = present_mode,
      .clipped = VK_TRUE};

  swap_chain_ = device_->createSwapchainKHRUnique(create_info, nullptr, dldi_);

  auto const swap_chain_images =
      device_->getSwapchainImagesKHR(swap_chain_.get(), dldi_);

  swap_chain_frames_.resize(swap_chain_images.size());
  for (auto i = 0u; i < swap_chain_images.size(); i++) {
    swap_chain_frames_[i].image = swap_chain_images[i];
  }

  swap_chain_image_format_ = surface_format.format;
  swap_chain_image_extent_ = extent;
}

void VulkanEngine::createImageViews() {
  for (std::size_t i = 0u; i < swap_chain_frames_.size(); i++) {
    vk::ImageViewCreateInfo create_info{
        .image = swap_chain_frames_[i].image,
        .viewType = vk::ImageViewType::e2D,
        .format = swap_chain_image_format_,
        .subresourceRange = {
            .aspectMask = vk::ImageAspectFlagBits::eColor,
            .baseMipLevel = 0u,
            .levelCount = 1u,
            .baseArrayLayer = 0u,
            .layerCount = 1u,
        }};
    swap_chain_frames_[i].image_view =
        device_->createImageViewUnique(create_info, nullptr, dldi_);
  }
}

void VulkanEngine::createRenderPass() {
  // Color attachment
  vk::AttachmentDescription color_attachment{
      .format = swap_chain_image_format_,
      .samples = vk::SampleCountFlagBits::e1,
      .loadOp = vk::AttachmentLoadOp::eClear,
      .storeOp = vk::AttachmentStoreOp::eStore,
      .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
      .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
      .initialLayout = vk::ImageLayout::eUndefined,
      .finalLayout = vk::ImageLayout::ePresentSrcKHR};

  vk::AttachmentReference color_attachment_ref{};
  color_attachment_ref.attachment = 0u;
  color_attachment_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;

  // Depth attachment
  vk::AttachmentDescription depth_attachment{};
  depth_attachment.format = vk::Format::eD32Sfloat;
  depth_attachment.samples = vk::SampleCountFlagBits::e1;
  depth_attachment.loadOp = vk::AttachmentLoadOp::eClear;
  depth_attachment.storeOp = vk::AttachmentStoreOp::eStore;
  depth_attachment.stencilLoadOp = vk::AttachmentLoadOp::eClear;
  depth_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  depth_attachment.initialLayout = vk::ImageLayout::eUndefined;
  depth_attachment.finalLayout =
      vk::ImageLayout::eDepthStencilAttachmentOptimal;

  vk::AttachmentReference depth_attachment_ref{
      .attachment = 1u,
      .layout = vk::ImageLayout::eDepthStencilAttachmentOptimal,
  };

  // Subpasses
  vk::SubpassDescription subpass{
      .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
      .colorAttachmentCount = 1u,
      .pColorAttachments = &color_attachment_ref,
      .pDepthStencilAttachment = &depth_attachment_ref,
  };

  vk::SubpassDependency dependency{
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0u,
      .srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput |
                      vk::PipelineStageFlagBits::eEarlyFragmentTests,
      .dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput |
                      vk::PipelineStageFlagBits::eEarlyFragmentTests,
      .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite |
                       vk::AccessFlagBits::eDepthStencilAttachmentWrite,
  };

  std::array<vk::AttachmentDescription, 2> attachments{color_attachment,
                                                       depth_attachment};

  vk::RenderPassCreateInfo create_info{
      .flags = {},
      .attachmentCount = static_cast<std::uint32_t>(attachments.size()),
      .pAttachments = attachments.data(),
      .subpassCount = 1u,
      .pSubpasses = &subpass,
      .dependencyCount = 1u,
      .pDependencies = &dependency};

  render_pass_ = device_->createRenderPassUnique(create_info, nullptr, dldi_);
}

void VulkanEngine::createDescriptorSetLayout() {
  vk::DescriptorSetLayoutBinding ubo_layout_binding{
      .binding = 0u,
      .descriptorType = vk::DescriptorType::eUniformBuffer,
      .descriptorCount = 1u,
      .stageFlags = vk::ShaderStageFlagBits::eVertex,
  };

  vk::DescriptorSetLayoutCreateInfo layout_info{
      .bindingCount = 1u,
      .pBindings = &ubo_layout_binding,
  };

  uniform_descriptor_set_layout_ =
      device_->createDescriptorSetLayoutUnique(layout_info, nullptr, dldi_);

  vk::DescriptorSetLayoutBinding global_layout_binding{
      .binding = 0u,
      .descriptorType = vk::DescriptorType::eStorageBuffer,
      .descriptorCount = 1u,
      .stageFlags = vk::ShaderStageFlagBits::eVertex,
  };

  layout_info.bindingCount = 1u;
  layout_info.pBindings = &global_layout_binding;

  global_decriptor_set_layout_ =
      device_->createDescriptorSetLayoutUnique(layout_info, nullptr, dldi_);
}

void VulkanEngine::createPipelineFactory() {}
