#include "json_utils.hpp"

#include <boost/json/detail/value_to.hpp>
#include <boost/json/object.hpp>

vk::Extent2D vk::tag_invoke(json::value_to_tag<vk::Extent2D>,
                            json::value const& jv) {
  vk::Extent2D extent{};
  if (jv.is_object()) {
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
  }
  return extent;
}

vk::Flags<vk::DebugUtilsMessageSeverityFlagBitsEXT> vk::tag_invoke(
    json::value_to_tag<vk::Flags<vk::DebugUtilsMessageSeverityFlagBitsEXT>>,
    json::value const& jv) {
  vk::Flags<vk::DebugUtilsMessageSeverityFlagBitsEXT> severity_flags;
  if (jv.is_object()) {
    json::object const& obj = jv.get_object();
    if (auto p = obj.if_contains("verbose")) {
      if (p->is_bool() && p->get_bool()) {
        severity_flags |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
      }
    }
    if (auto p = obj.if_contains("info")) {
      if (p->is_bool() && p->get_bool()) {
        severity_flags |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
      }
    }
    if (auto p = obj.if_contains("warning")) {
      if (p->is_bool() && p->get_bool()) {
        severity_flags |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
      }
    }
    if (auto p = obj.if_contains("error")) {
      if (p->is_bool() && p->get_bool()) {
        severity_flags |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
      }
    }
  }
  return severity_flags;
}
