#ifndef VK_TYPES_HPP
#define VK_TYPES_HPP

#include <vk_mem_alloc.h>

#include "../utils/include_vulkan.hpp"

struct AllocatedBuffer {
  vk::Buffer buffer;
  VmaAllocation allocation;
  VmaAllocator allocator;

  inline ~AllocatedBuffer() {
    if (allocation) {
      vmaDestroyBuffer(allocator, buffer, allocation);
    }
  };
};

struct VmaAllocatorPtrDestroyer {
  void operator()(VmaAllocator vma_allocator) {
    if (vma_allocator) {
      vmaDestroyAllocator(vma_allocator);
    }
  }
};

#endif  // VK_TYPES_HPP