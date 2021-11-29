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
  Shader(const vk::Device device, const vk::DispatchLoaderDynamic& dldi_,
         const ShaderResource& resource);
  vk::ShaderModuleCreateInfo GetShaderModuleCreateInfo() const;
  inline const std::string& GetName() const { return resource_.name; }
  inline vk::ShaderStageFlagBits GetType() const {
    switch (resource_.type) {
      case ShaderType::Fragment:
        return vk::ShaderStageFlagBits::eFragment;
      case ShaderType::Vertex:
        return vk::ShaderStageFlagBits::eVertex;
    }
    return {};
  }
  inline vk::ShaderModule GetModule() const { return module_.get(); }

 private:
  vk::UniqueHandle<vk::ShaderModule, vk::DispatchLoaderDynamic> module_;
  ShaderResource const resource_;
};
}  // namespace sc
