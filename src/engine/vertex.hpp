#pragma once

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vulkan/vulkan.hpp>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 color;

  struct VertexInputDescription {
    vk::VertexInputBindingDescription binding;
    std::array<vk::VertexInputAttributeDescription, 3> attributes;
  };

  static VertexInputDescription getVertexDescription();
};
