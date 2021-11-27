#pragma once

#include <cstdint>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

#include "sc_resource.hpp"

namespace sc {
enum struct ShaderType { Fragment, Vertex };

struct ShaderResource : Resource {
  ShaderType type;
  std::vector<std::uint32_t> buffer;
};

class Shader {
 public:
  Shader(const vk::Device& device, ShaderResource resource);
  vk::ShaderModuleCreateInfo GetShaderModuleCreateInfo() const;

 private:
  vk::UniqueHandle<vk::ShaderModule, vk::DispatchLoaderDynamic> module_;
  ShaderResource const resource_;
};
}  // namespace sc
