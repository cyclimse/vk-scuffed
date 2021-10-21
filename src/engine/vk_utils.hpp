#include <iostream>

#include "../utils/include_vulkan.hpp"

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

vk::DebugUtilsMessengerCreateInfoEXT GetDebugUtilsMessengerCreateInfoStruct();