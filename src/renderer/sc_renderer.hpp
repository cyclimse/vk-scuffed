#pragma once

#include <GLFW/glfw3.h>

#include "sc_assets.hpp"
#include "sc_config.hpp"
#include "sc_window.hpp"
#include "vk_engine.hpp"

namespace sc {
struct InitGLFW {
  inline InitGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  }
#ifndef __SANITIZE_ADDRESS__
  // AFAIK glfwTerminate calls dlclose which causes false positives with ASAN
  inline ~InitGLFW() { glfwTerminate(); }
#endif
};

class Renderer {
 public:
  Renderer(Config const *cfg, Assets const *assets, void *user_ptr);

  Config const *cfg_;
  Assets const *assets_;

  InitGLFW init_glfw_;
  Window window_;
  VulkanEngine engine_;
};
}  // namespace sc
