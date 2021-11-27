#include "pipeline_factory.hpp"

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <vulkan/vulkan_enums.hpp>
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
    const sc::Material& material) {
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

  vk::PipelineDepthStencilStateCreateInfo depth_stencil{
      .depthTestEnable = true,
      .depthWriteEnable = true,
      .depthCompareOp = vk::CompareOp::eLess,
      .depthBoundsTestEnable = false,
      .stencilTestEnable = false};

  vk::PipelineColorBlendAttachmentState color_blend_attachment{
      .srcColorBlendFactor = vk::BlendFactor::eOne,
      .dstColorBlendFactor = vk::BlendFactor::eZero,
      .colorBlendOp = vk::BlendOp::eAdd,
      .srcAlphaBlendFactor = vk::BlendFactor::eOne,
      .dstAlphaBlendFactor = vk::BlendFactor::eZero,
      .alphaBlendOp = vk::BlendOp::eAdd,
      .colorWriteMask =
          vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};

  vk::PipelineColorBlendStateCreateInfo color_blending{
      .logicOpEnable = false,
      .logicOp = vk::LogicOp::eCopy,
      .attachmentCount = 1u,
      .pAttachments = &color_blend_attachment};

  std::vector<vk::DynamicState> dynamic_state_attachments = {
      vk::DynamicState::eViewport, vk::DynamicState::eScissor};

  vk::PipelineDynamicStateCreateInfo dynamic_state{
      .dynamicStateCount =
          static_cast<std::uint32_t>(dynamic_state_attachments.size()),
      .pDynamicStates = dynamic_state_attachments.data()};

  auto shader_stages = material.GetShaderStageCreateInfos();

  vk::GraphicsPipelineCreateInfo create_info{
      .flags = {},
      .stageCount = static_cast<std::uint32_t>(shader_stages.size()),
      .pStages = shader_stages.data(),
      .pVertexInputState = &vertex_input_info_,
      .pInputAssemblyState = &input_assembly,
      .pTessellationState = nullptr,
      .pViewportState = &view_port_state,
      .pRasterizationState = &rasterizer_state,
      .pMultisampleState = &multisampling_state,
      .pDepthStencilState = &depth_stencil,
      .pColorBlendState = &color_blending,
      .pDynamicState = &dynamic_state,
      .layout = layout_,
      .renderPass = pass_,
      .subpass = 0};

  return create_info;
}
