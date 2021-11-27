#pragma once

#include <cstdint>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_structs.hpp>

#include "sc_resource.hpp"

namespace sc {
enum struct ShaderType { Fragment, Vertex };

struct Shader : Resource {
  vk::ShaderModuleCreateInfo GetShaderModuleCreateInfo() const;
  ShaderType type;
  std::vector<std::uint32_t> buffer;
};
}  // namespace sc
