#include "vk_utils.hpp"

#include "queue_family_indices.hpp"
#include "swap_chain_support.hpp"

VKAPI_ATTR VkBool32 VKAPI_CALL
DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
              [[maybe_unused]] void* pUserData) {
  if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
  } else {
    std::clog << "\t" << pCallbackData->pMessage << std::endl;
  }
  return VK_FALSE;
}

vk::DebugUtilsMessengerCreateInfoEXT GetDebugUtilsMessengerCreateInfoStruct() {
  auto const severityFlags =
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      // vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;

  auto const typeFlags = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                         vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                         vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

  return vk::DebugUtilsMessengerCreateInfoEXT{.flags = {},
                                              .messageSeverity = severityFlags,
                                              .messageType = typeFlags,
                                              .pfnUserCallback = DebugCallBack};
}

bool IsDeviceSuitable(const vk::PhysicalDevice& device,
                      const vk::SurfaceKHR& surface) {
  auto const properties = device.getProperties();
  auto const features = device.getFeatures();
  auto const extensions = device.enumerateDeviceExtensionProperties();
#ifndef NDEBUG
  std::clog << "Evaluating suitability of device: " << properties.deviceName
            << std::endl;
#endif

  std::vector<const char*> device_extensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  for (auto const& required_extension : device_extensions) {
    auto const result = std::find_if(
        extensions.cbegin(), extensions.cend(),
        [&](const vk::ExtensionProperties& extension) {
          return std::strcmp(extension.extensionName, required_extension) == 0;
        });
    if (result == extensions.cend()) {
#ifndef NDEBUG
      std::clog << "Extension " << required_extension
                << "was required but could not be found!" << std::endl;
#endif
      // A required extension could not be found.
      return false;
    }
  }

  SwapChainSupport support{device, surface, {}};  // Extent is not used
  QueueFamilyIndices indices{device, surface};

  return support.HasRequiredSupport() && indices.IsComplete() &&
         features.geometryShader &&
         properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
}
