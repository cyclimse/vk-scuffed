#pragma once

#include <stdint.h>  // for uint32_t

#include <boost/config/detail/suffix.hpp>  // for boost
#include <boost/json.hpp>
#include <boost/json/value.hpp>  // for value
#include <filesystem>            // for path
#include <string>                // for allocator, char_traits
#include <vulkan/vulkan.hpp>     // for DebugUtilsMessageSeverityF...

namespace boost {
namespace json {
class stream_parser;
template <class T>
struct value_to_tag;
}  // namespace json
}  // namespace boost

using namespace boost;

namespace sc {
struct Config {
  static Config Default();
  static Config Load(json::stream_parser& parser,
                     std::filesystem::path const& config_file);

  // Global config
  std::uint32_t version;
  bool is_debug;
  std::string release_type;
  std::string project_name;

  // Engine config
  struct Engine {
    std::string engine_name;
    vk::Extent2D resolution;
    bool validation_layers_enabled;
    vk::Flags<vk::DebugUtilsMessageSeverityFlagBitsEXT> severity_flags;
  };
  Engine eng;

  inline std::string GetWindowName() const {
    return project_name + '_' + std::to_string(version) + '_' + release_type;
  }
};

Config tag_invoke(json::value_to_tag<sc::Config>, json::value const& jv);
}  // namespace sc
