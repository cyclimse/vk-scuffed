#pragma once

#include <boost/json.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "sc_resource.hpp"

using namespace boost;

namespace sc {

struct Material : Resource {
  std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStageCreateInfos()
      const;
  std::vector<std::string> shaders;
};

Material tag_invoke(json::value_to_tag<Material>, json::value const& jv);
}  // namespace sc
