#pragma once

#include <GLFW/glfw3.h>

#include "sc_config.hpp"
#include "sc_window.hpp"
#include "vk_engine.hpp"

namespace sc {
class Renderer {
 public:
  Renderer(std::shared_ptr<Config> const cfg, void *user_ptr);

  std::shared_ptr<Config> const cfg_;
  Window window_;
  VulkanEngine engine_;
};
}  // namespace sc