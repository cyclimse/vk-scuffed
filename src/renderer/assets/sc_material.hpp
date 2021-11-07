#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace sc {
class Material {
 public:
  std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStageCreateInfos() const;
};
}  // namespace sc