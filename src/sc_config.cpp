#include "sc_config.hpp"

#include <boost/json/detail/value_to.hpp>
#include <boost/json/object.hpp>
#include <boost/json/stream_parser.hpp>
#include <boost/json/string.hpp>
#include <boost/json/system_error.hpp>
#include <boost/json/value_to.hpp>
#include <exception>
#include <filesystem>
#include <fstream>   // IWYU pragma: keep
#include <iostream>  // IWYU pragma: keep
#include <sstream>
#include <string>
#include <vulkan/vulkan.hpp>

#include "json_utils.hpp"

using namespace sc;

Config Config::Default() {
#ifndef NDEBUG
  constexpr bool is_debug = true;
#else
  constexpr bool is_debug = false;
#endif

  return Config{
      .version = 1u,
      .is_debug = is_debug,
      .release_type = is_debug ? "DEBUG" : "RELEASE",
      .project_name = "Scuffecraft",
      .eng{.engine_name = "Scuffedcraft engine",
           .resolution = {1280, 720},
           .validation_layers_enabled = is_debug,
           .severity_flags = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError}};
}

Config Config::Load(json::stream_parser& parser,
                    std::filesystem::path const& config_file) {
  if (!std::filesystem::exists(config_file)) {
    return Config::Default();
  } else {
    json::error_code error_code;

    std::ifstream stream{config_file};
    std::string line;
    while (std::getline(stream, line)) {
      parser.write(line, error_code);
      if (error_code) break;
    }
    parser.finish(error_code);
    if (error_code) {
      std::clog << "[ERROR] Parsing failed for " << config_file << " : "
                << error_code.message() << std::endl;
      return Config::Default();
    }

    auto jv = parser.release();
    parser.reset();
    try {
      return json::value_to<Config>(jv);
    } catch (std::exception const& e) {
      std::clog << "[ERROR] Could not create Material " << config_file
                << " from json " << e.what() << std::endl;
      return Config::Default();
    }
  }
}

Config sc::tag_invoke(json::value_to_tag<Config>, json::value const& jv) {
  Config config = Config::Default();

  json::object const& obj = jv.as_object();
  if (auto p = obj.if_contains("project_name")) {
    if (p->is_string()) {
      config.project_name = p->get_string().c_str();
    }
  }
  if (auto p = obj.if_contains("engine")) {
    if (p->is_object()) {
      json::object const& eng_obj = p->get_object();
      if (auto pp = eng_obj.if_contains("resolution")) {
        config.eng.resolution = json::value_to<vk::Extent2D>(*pp);
      }
      if (auto pp = eng_obj.if_contains("messenger_severity_flags")) {
        auto const flags =
            json::value_to<vk::Flags<vk::DebugUtilsMessageSeverityFlagBitsEXT>>(
                *pp);
        config.eng.severity_flags = flags ? flags : config.eng.severity_flags;
      }
    }
  }

  return config;
}
