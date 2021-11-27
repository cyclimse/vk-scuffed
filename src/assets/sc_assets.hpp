#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include "sc_material.hpp"
#include "sc_shader.hpp"

namespace sc {
struct Assets {
  static Assets Load(json::stream_parser& parser,
                     std::filesystem::path const& directory);
  std::unordered_map<std::string, Material> materials;
  std::unordered_map<std::string, Shader> shaders;
};
}  // namespace sc
