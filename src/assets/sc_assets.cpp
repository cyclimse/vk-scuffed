#include "sc_assets.hpp"

#include <boost/json/stream_parser.hpp>
#include <boost/json/system_error.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_to.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>   // IWYU pragma: keep
#include <iostream>  // IWYU pragma: keep
#include <sstream>
#include <utility>
#include <vector>

#include "sc_material.hpp"
#include "sc_resource.hpp"
#include "sc_shader.hpp"

using namespace sc;
using namespace boost;

Assets Assets::Load(json::stream_parser& parser,
                    std::filesystem::path const& directory) {
  // TODO: This function will be completely redone with some sort of scripting
  // integration
  using std::filesystem::directory_iterator;

  Assets assets{};
  json::error_code error_code;

  for (const auto& file : directory_iterator(directory / "materials")) {
    if (file.is_regular_file() && file.path().extension() == ".json") {
      std::ifstream stream{file.path()};
      std::string line;
      while (std::getline(stream, line)) {
        parser.write(line, error_code);
        if (error_code) break;
      }
      parser.finish(error_code);
      if (error_code) {
        std::clog << "[ERROR] Parsing failed for " << file.path() << " : "
                  << error_code.message() << std::endl;
        break;
      }
      auto jv = parser.release();
      parser.reset();
      try {
        MaterialResource material = json::value_to<MaterialResource>(jv);
        assets.materials[material.name] = std::move(material);
      } catch (std::exception const& e) {
        std::clog << "[ERROR] Could not create Material " << file.path()
                  << " from json " << e.what() << std::endl;
        break;
      }
    }
  }

  for (const auto& file : directory_iterator(directory / "shaders")) {
    if (file.is_regular_file() && file.path().extension() == ".spv") {
      std::ifstream stream(file.path(), std::ios::in | std::ios::binary);

      if (!stream.good()) {
        std::clog << "[WARNING] Could not load shader " << file.path()
                  << std::endl;
        break;
      }

      size_t file_size = static_cast<size_t>(stream.tellg());
      ShaderResource shader{};

      shader.buffer.reserve(file_size / sizeof(std::uint32_t));
      stream.seekg(0);

      stream.read(reinterpret_cast<char*>(shader.buffer.data()), file_size);

      stream.close();

      auto name = file.path().filename().replace_extension();
      shader.name = name;
      if (name.extension() == ".vert") {
        shader.type = ShaderType::Vertex;
      } else if (name.extension() == ".frag") {
        shader.type = ShaderType::Fragment;
      } else {
        std::clog << "[WARNING] Unkown shader extension on " << name
                  << std::endl;
        break;
      }
      assets.shaders[shader.name] = std::move(shader);
    }
  }

  for (auto iter = assets.materials.cbegin(); iter != assets.materials.cend();
       iter++) {
    std::clog << "[INFO] Loaded material " << iter->second.name << std::endl;
  }
  for (auto iter = assets.shaders.cbegin(); iter != assets.shaders.cend();
       iter++) {
    std::clog << "[INFO] Loaded shader " << iter->second.name << std::endl;
  }

  return assets;
}
