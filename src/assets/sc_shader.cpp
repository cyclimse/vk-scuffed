#include "sc_shader.hpp"

#include <cstddef>
#include <cstdint>
#include <vulkan/vulkan_structs.hpp>

using namespace sc;

Shader::Shader(const vk::Device device, const vk::DispatchLoaderDynamic& dldi,
               const ShaderResource& resource)
    : resource_{resource} {
  module_ = device.createShaderModuleUnique(GetShaderModuleCreateInfo(),
                                            nullptr, dldi);
}

vk::ShaderModuleCreateInfo Shader::GetShaderModuleCreateInfo() const {
  return vk::ShaderModuleCreateInfo{
      .codeSize = resource_.buffer.size() * sizeof(std::uint32_t),
      .pCode = resource_.buffer.data()};
}
