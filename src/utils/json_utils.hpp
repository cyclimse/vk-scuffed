#pragma once

#include <boost/config/detail/suffix.hpp>  // for boost
#include <boost/json.hpp>
#include <boost/json/value.hpp>  // for value
#include <vulkan/vulkan.hpp>     // for DebugUtilsMessageSeverityF...
#include <vulkan/vulkan_structs.hpp>

namespace boost {
namespace json {
template <class T>
struct value_to_tag;
}  // namespace json
}  // namespace boost

using namespace boost;

namespace vk {
vk::Extent2D tag_invoke(json::value_to_tag<vk::Extent2D>,
                        json::value const& jv);
vk::Flags<vk::DebugUtilsMessageSeverityFlagBitsEXT> tag_invoke(
    json::value_to_tag<vk::Flags<vk::DebugUtilsMessageSeverityFlagBitsEXT>>,
    json::value const& jv);
}  // namespace vk
