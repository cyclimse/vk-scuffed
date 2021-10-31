#ifndef QUEUEFAMILYINDICES_HPP
#define QUEUEFAMILYINDICES_HPP

#include <vulkan/vulkan.hpp>

struct QueueFamilyIndices {
  std::optional<std::uint32_t> graphics_family;
  std::optional<std::uint32_t> present_family;

  QueueFamilyIndices() = default;

  QueueFamilyIndices(const vk::PhysicalDevice& device,
                     const vk::SurfaceKHR& surface);

  bool IsComplete() {
    return graphics_family.has_value() && present_family.has_value();
  }
};

#endif  // QUEUEFAMILYINDICES_HPP