#include "pipeline_factory.hpp"

#include <vulkan/vulkan_core.h>

#include <vulkan/vulkan_structs.hpp>

PipelineFactory::PipelineFactory(
    vk::RenderPass pass, vk::PipelineLayout layout,
    vk::PipelineVertexInputStateCreateInfo vertex_input_info,
    vk::Extent2D extent)
    : pass_{pass},
      layout_{layout},
      vertex_input_info_{vertex_input_info},
      extent_{extent} {};

vk::GraphicsPipelineCreateInfo PipelineFactory::Build(
    const sc::Material &material) {
  vk::PipelineInputAssemblyStateCreateInfo input_assembly{
      .flags = {},
      .topology = vk::PrimitiveTopology::eTriangleList,
      .primitiveRestartEnable = false};

  vk::Viewport viewport{.x = 0.0f,
                        .y = 0.0f,
                        .width = static_cast<float>(extent_.width),
                        .height = static_cast<float>(extent_.width),
                        .minDepth = 0.0f,
                        .maxDepth = 1.0f};

  vk::Rect2D scissor{.offset = {0, 0}, .extent = extent_};

  vk::PipelineViewportStateCreateInfo view_port_state{.flags = {},
                                                      .viewportCount = 1u,
                                                      .pViewports = &viewport,
                                                      .scissorCount = 1u,
                                                      .pScissors = &scissor};

  vk::PipelineRasterizationStateCreateInfo rasterizer_state{
      .flags = {},
      .depthClampEnable = false,
      .rasterizerDiscardEnable = false,
      .polygonMode = vk::PolygonMode::eFill,
      .cullMode = {},
      .frontFace = vk::FrontFace::eClockwise,
      .depthBiasEnable = {},
      .depthBiasConstantFactor = {},
      .depthBiasClamp = {},
      .depthBiasSlopeFactor = {},
      .lineWidth = 1.0f};

  vk::PipelineMultisampleStateCreateInfo multisampling_state{
      .flags = {},
      .rasterizationSamples = vk::SampleCountFlagBits::e1,
      .sampleShadingEnable = false,
      .minSampleShading = 1.0,
      .pSampleMask = nullptr,
      .alphaToCoverageEnable = false,
      .alphaToOneEnable = false};

  vk::GraphicsPipelineCreateInfo create_info{};

  return create_info;
}