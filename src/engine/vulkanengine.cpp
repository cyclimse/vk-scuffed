#include "vulkanengine.hpp"

#include <vector>
#include <vulkan/vulkan_handles.hpp>

#include "../utils/constants.hpp"
#include "../utils/include_glfw.hpp"
#include "vk_utils.hpp"

VulkanEngine::VulkanEngine(Window const *window_ptr) : window_ptr_(window_ptr) {
  createInstance();
  createSurface();
}

void VulkanEngine::createInstance() {
  auto glfw_extension_count = 0u;
  auto const glfw_extensions =
      glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  std::vector<const char *> extensions{glfw_extensions,
                                       glfw_extensions + glfw_extension_count};
  std::vector<const char *> layers{};

  if constexpr (Constants::kEnableValidationLayer) {
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

  if constexpr (Constants::kEnableValidationLayer) {
    vk::StructureChain<vk::InstanceCreateInfo,
                       vk::DebugUtilsMessengerCreateInfoEXT>
        chain = {instance_create_info,
                 GetDebugUtilsMessengerCreateInfoStruct()};
    instance_ = vk::createInstanceUnique(chain.get<vk::InstanceCreateInfo>());
  } else {
    instance_ = vk::createInstanceUnique(instance_create_info);
  }

  dldi_ = vk::DispatchLoaderDynamic(instance_.get(), vkGetInstanceProcAddr);

  if constexpr (Constants::kEnableValidationLayer) {
    messenger_ = instance_->createDebugUtilsMessengerEXTUnique(
        GetDebugUtilsMessengerCreateInfoStruct(), nullptr, dldi_);
  }
}

void VulkanEngine::createSurface() {
  surface_ = vk::UniqueSurfaceKHR(window_ptr_->CreateSurface(instance_.get()),
                                  instance_.get());
}