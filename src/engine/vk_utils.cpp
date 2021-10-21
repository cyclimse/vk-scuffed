#include "vk_utils.hpp"

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