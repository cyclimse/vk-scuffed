#pragma once
#include <vk_mem_alloc.h>

#include <vulkan/vulkan.hpp>

struct AllocatedBuffer {
  vk::Buffer buffer;
  VmaAllocation allocation = nullptr;
  VmaAllocator allocator = nullptr;

  inline ~AllocatedBuffer() {
    if (allocation) {
      vmaDestroyBuffer(allocator, buffer, allocation);
    }
  };
};

struct SwapChainFrame {
  vk::Image image;
  vk::UniqueImageView image_view;
  vk::UniqueFramebuffer frame_buffer;
  AllocatedBuffer uniform_buffer;
  AllocatedBuffer storage_buffer;
  vk::CommandBuffer command_buffer;
};

struct VmaAllocatorPtrDestroyer {
  void operator()(VmaAllocator vma_allocator) {
    if (vma_allocator) {
      vmaDestroyAllocator(vma_allocator);
    }
  }
};