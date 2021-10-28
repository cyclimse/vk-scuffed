#ifndef QUEUEFAMILYINDICES_HPP
#define QUEUEFAMILYINDICES_HPP

#include "../utils/include_vulkan.hpp"

struct QueueFamilyIndices {
  std::optional<std::uint32_t> graphics_family;
  std::optional<std::uint32_t> present_family;

  QueueFamilyIndices(const vk::PhysicalDevice& device,
                     const vk::SurfaceKHR& surface);

  bool IsComplete() {
    return graphics_family.has_value() && present_family.has_value();
  }
};

#endif  // QUEUEFAMILYINDICES_HPP