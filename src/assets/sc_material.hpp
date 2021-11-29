#pragma once

#include <boost/config/detail/suffix.hpp>  // for boost
#include <boost/json.hpp>
#include <boost/json/value.hpp>  // for value
#include <string>                // for string
#include <unordered_map>
#include <vector>  // for vector
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

#include "sc_resource.hpp"  // for Resource
#include "sc_shader.hpp"

namespace boost {
namespace json {
template <class T>
struct value_to_tag;
}  // namespace json
}  // namespace boost
namespace vk {
struct PipelineShaderStageCreateInfo;
}  // namespace vk

using namespace boost;

namespace sc {
struct MaterialResource : Resource {
  std::vector<std::string> shader_names;
};

class Material {
 public:
  Material(std::vector<sc::Shader const*>&& shaders,
           const MaterialResource& resource);
  std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStageCreateInfos()
      const;
  vk::UniqueHandle<vk::Pipeline, vk::DispatchLoaderDynamic> pipeline;
  vk::PipelineLayout pipeline_layout;

 private:
  std::vector<sc::Shader const*> shaders_;
  MaterialResource const resource_;
};

MaterialResource tag_invoke(json::value_to_tag<MaterialResource>,
                            json::value const& jv);
}  // namespace sc
