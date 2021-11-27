#include "sc_material.hpp"

#include <boost/json/array.hpp>
#include <boost/json/detail/value_to.hpp>
#include <boost/json/object.hpp>
#include <boost/json/string.hpp>
#include <boost/json/value_to.hpp>
#include <vulkan/vulkan.hpp>

using namespace sc;

std::vector<vk::PipelineShaderStageCreateInfo>
Material::GetShaderStageCreateInfos() const {
  vk::PipelineShaderStageCreateInfo shader{};

  return {};
}

MaterialResource sc::tag_invoke(json::value_to_tag<MaterialResource>,
                                json::value const& jv) {
  MaterialResource material{};

  json::object const& obj = jv.as_object();
  if (auto p = obj.if_contains("name")) {
    if (p->is_string()) {
      material.name = p->get_string().c_str();
    }
  }
  if (auto p = obj.if_contains("shaders")) {
    if (p->is_array()) {
      material.shader_names = json::value_to<std::vector<std::string>>(*p);
    }
  }
  return material;
}
