#include "sc_renderer.hpp"

#include <GLFW/glfw3.h>

using namespace sc;

Renderer::Renderer(std::shared_ptr<Config> const cfg, void *user_ptr)
    : cfg_{cfg},
      init_glfw_{},
      window_{cfg_->eng.resolution, cfg_->window_name, user_ptr},
      engine_{cfg_, &window_} {}