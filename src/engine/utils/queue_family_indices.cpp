#include "queue_family_indices.hpp"

QueueFamilyIndices::QueueFamilyIndices(const vk::PhysicalDevice& device,
                                       const vk::SurfaceKHR& surface) {
  auto const queue_families = device.getQueueFamilyProperties();

  auto i = 0u;
  for (const auto& queue : queue_families) {
    if (queue.queueFlags & vk::QueueFlagBits::eGraphics) {
      graphics_family = i;
    }

    vk::Bool32 p_support{};
    if (device.getSurfaceSupportKHR(i, surface, &p_support) !=
        vk::Result::eSuccess) {
      throw std::runtime_error("toto");
    }

    if (p_support) {
      present_family = i;
    }

    if (IsComplete()) {
      break;
    }

    i++;
  }
}
