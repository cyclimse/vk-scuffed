#ifndef VK_UTILS_HPP
#define VK_UTILS_HPP

#include <iostream>

#include "../utils/constants.hpp"
#include "queue_family_indices.hpp"
#include "swap_chain_support.hpp"

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

vk::DebugUtilsMessengerCreateInfoEXT GetDebugUtilsMessengerCreateInfoStruct();

bool IsDeviceSuitable(const vk::PhysicalDevice& device,
                      const vk::SurfaceKHR& surface);

#endif  // VK_UTILS_HPP