#pragma once

#include <string>
#include <string_view>
#include <vulkan/vulkan.hpp>

namespace sc {
struct Config {
  Config();

  // Global config
  std::uint32_t const version;
  bool const is_debug;
  std::string const release_type;
  std::string const project_name;
  std::string const window_name;

  // Engine config
  struct Engine {
    std::string const engine_name;
    vk::Extent2D resolution;
    bool const validation_layers_enabled;
  };
  Engine eng;
};
}  // namespace sc