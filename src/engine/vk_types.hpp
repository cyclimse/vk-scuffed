#pragma once
#include <vk_mem_alloc.h>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

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
  vk::UniqueHandle<vk::ImageView, vk::DispatchLoaderDynamic> image_view;
  vk::UniqueHandle<vk::Framebuffer, vk::DispatchLoaderDynamic> frame_buffer;
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
