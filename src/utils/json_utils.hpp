#pragma once

#include <boost/json.hpp>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_structs.hpp>

using namespace boost;

namespace vk {
vk::Extent2D tag_invoke(json::value_to_tag<vk::Extent2D>,
                        json::value const& jv);
}
