#pragma once

#include <string_view>
#include <unordered_map>

#include "sc_material.hpp"

namespace sc {
struct Assets {
  static Assets Load();
  std::unordered_map<std::string_view, Material> materials;
};
}  // namespace sc