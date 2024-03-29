#pragma once

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_structs.hpp>

#include "sc_material.hpp"

class PipelineFactory {
 public:
  PipelineFactory() = default;
  PipelineFactory(vk::RenderPass pass, vk::PipelineLayout layout,
                  vk::PipelineVertexInputStateCreateInfo vertex_input_info,
                  vk::Extent2D extent);
  void Build(vk::Device device, vk::DispatchLoaderDynamic dldi,
             sc::Material& material);

 private:
  vk::RenderPass pass_;
  vk::PipelineLayout layout_;
  vk::PipelineVertexInputStateCreateInfo vertex_input_info_;
  vk::Extent2D extent_;
};
