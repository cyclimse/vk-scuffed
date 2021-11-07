#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

vk::DebugUtilsMessengerCreateInfoEXT GetDebugUtilsMessengerCreateInfoStruct();

bool IsDeviceSuitable(const vk::PhysicalDevice& device,
                      const vk::SurfaceKHR& surface);
