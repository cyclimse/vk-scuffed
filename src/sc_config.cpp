#include "sc_config.hpp"

#include <string>
#include <vulkan/vulkan.hpp>

using namespace sc;

Config::Config()
    : version{1u},
#ifndef NDEBUG
      is_debug{true},
#else
      is_debug{false},
#endif
      release_type{is_debug ? "DEBUG" : "RELEASE"},
      project_name{"Scuffedcraft"},
      window_name{project_name + '_' + std::to_string(version) + '_' +
                  release_type},
      eng{.engine_name = "Scuffedcraft engine",
          .resolution = {1280, 720},
          .validation_layers_enabled = is_debug} {
}