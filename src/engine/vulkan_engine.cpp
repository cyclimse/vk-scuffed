#include "vulkan_engine.hpp"

#include <set>
#include <vector>

#include "../utils/constants.hpp"
#include "../utils/include_glfw.hpp"
#include "vk_utils.hpp"

VulkanEngine::VulkanEngine(Window const *window_ptr) : window_ptr_(window_ptr) {
  createInstance();
  createSurface();
  pickPhysicalDevice();
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

  if constexpr (Constants::Engine::kEnableValidationLayer) {
    vk::StructureChain<vk::InstanceCreateInfo,
                       vk::DebugUtilsMessengerCreateInfoEXT>
        chain = {instance_create_info,
                 GetDebugUtilsMessengerCreateInfoStruct()};
    instance_ = vk::createInstanceUnique(chain.get<vk::InstanceCreateInfo>());
  } else {
    instance_ = vk::createInstanceUnique(instance_create_info);
  }

  dldi_ = vk::DispatchLoaderDynamic(instance_.get(), vkGetInstanceProcAddr);

  if constexpr (Constants::Engine::kEnableValidationLayer) {
    messenger_ = instance_->createDebugUtilsMessengerEXTUnique(
        GetDebugUtilsMessengerCreateInfoStruct(), nullptr, dldi_);
  }
}

void VulkanEngine::createSurface() {
  surface_ = vk::UniqueSurfaceKHR(window_ptr_->CreateSurface(instance_.get()),
                                  instance_.get());
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

  device_ = physical_device_.createDeviceUnique(create_info);

  graphics_queue_ = device_->getQueue(indices.graphics_family.value(), 0);
  presentation_queue_ = device_->getQueue(indices.present_family.value(), 0);
}