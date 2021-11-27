#include "sc_shader.hpp"

#include <cstdint>
#include <vulkan/vulkan_structs.hpp>

using namespace sc;

vk::ShaderModuleCreateInfo Shader::GetShaderModuleCreateInfo() const {
  return vk::ShaderModuleCreateInfo{
      .codeSize = resource_.buffer.size() * sizeof(std::uint32_t),
      .pCode = resource_.buffer.data()};
}
