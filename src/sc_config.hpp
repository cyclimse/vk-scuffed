#pragma once

#include <boost/json.hpp>
#include <filesystem>
#include <string>
#include <vulkan/vulkan.hpp>

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
  };
  Engine eng;

  inline std::string GetWindowName() const {
    return project_name + '_' + std::to_string(version) + '_' + release_type;
  }
};

Config tag_invoke(json::value_to_tag<sc::Config>, json::value const& jv);
}  // namespace sc
