#pragma once

#include <boost/config/detail/suffix.hpp>  // for boost
#include <boost/json.hpp>
#include <filesystem>     // for path
#include <string>         // for string, hash
#include <unordered_map>  // for unordered_map

#include "sc_material.hpp"  // for MaterialResource
#include "sc_shader.hpp"    // for ShaderResource

namespace boost {
namespace json {
class stream_parser;
}  // namespace json
}  // namespace boost

using namespace boost;

namespace sc {
struct Assets {
  static Assets Load(json::stream_parser& parser,
                     std::filesystem::path const& directory);
  std::unordered_map<std::string, MaterialResource> materials;
  std::unordered_map<std::string, ShaderResource> shaders;
};
}  // namespace sc
