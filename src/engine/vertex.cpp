#include "vertex.hpp"

Vertex::VertexInputDescription Vertex::getVertexDescription() {
  VertexInputDescription description{};

  description.binding.binding = 0u;
  description.binding.stride = sizeof(Vertex);
  description.binding.inputRate = vk::VertexInputRate::eVertex;

  description.attributes[0].binding = 0u;
  description.attributes[0].location = 0u;
  description.attributes[0].format = vk::Format::eR32G32B32Sfloat;
  description.attributes[0].offset = offsetof(Vertex, position);

  description.attributes[1].binding = 0u;
  description.attributes[1].location = 1u;
  description.attributes[1].format = vk::Format::eR32G32B32Sfloat;
  description.attributes[1].offset = offsetof(Vertex, normal);

  description.attributes[2].binding = 0u;
  description.attributes[2].location = 2u;
  description.attributes[2].format = vk::Format::eR32G32B32Sfloat;
  description.attributes[2].offset = offsetof(Vertex, color);

  return description;
}
