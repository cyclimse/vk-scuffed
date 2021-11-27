#pragma once

#include <vulkan/vk_platform.h>  // for VKAPI_ATTR, VKAPI_CALL
#include <vulkan/vulkan_core.h>  // for VkBool32, VkDebugUtilsMessageSeverit...

#include <iostream>
#include <vulkan/vulkan.hpp>  // for DebugUtilsMessageSeverityFlagBitsEXT

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

vk::DebugUtilsMessengerCreateInfoEXT GetDebugUtilsMessengerCreateInfoStruct(
    vk::Flags<vk::DebugUtilsMessageSeverityFlagBitsEXT> severity_flags);

bool IsDeviceSuitable(const vk::PhysicalDevice& device,
                      const vk::SurfaceKHR& surface);
