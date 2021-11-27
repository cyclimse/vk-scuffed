#include "sc_material.hpp"

using namespace sc;

Material sc::tag_invoke(json::value_to_tag<Material>, json::value const& jv) {
  Material material{};

  json::object const& obj = jv.as_object();
  if (auto p = obj.if_contains("name")) {
    if (p->is_string()) {
      material.name = p->get_string().c_str();
    }
  }
  if (auto p = obj.if_contains("shaders")) {
    if (p->is_array()) {
      material.shaders = json::value_to<std::vector<std::string>>(*p);
    }
  }
  return material;
}
