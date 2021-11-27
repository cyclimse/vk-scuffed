#include "json_utils.hpp"

#include <cstdint>
#include <vulkan/vulkan_structs.hpp>

vk::Extent2D vk::tag_invoke(json::value_to_tag<vk::Extent2D>,
                            json::value const& jv) {
  vk::Extent2D extent{};

  json::object const& obj = jv.as_object();
  if (auto p = obj.if_contains("width")) {
    if (p->is_number()) {
      extent.width = p->get_int64();
    }
  }
  if (auto p = obj.if_contains("height")) {
    if (p->is_number()) {
      extent.height = p->get_int64();
    }
  }
  return extent;
}
