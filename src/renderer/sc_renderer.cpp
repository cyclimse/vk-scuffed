#include "sc_renderer.hpp"

#include <GLFW/glfw3.h>

using namespace sc;

Renderer::Renderer(Config const *cfg, Assets const *assets, void *user_ptr)
    : cfg_{cfg},
      assets_{assets},
      init_glfw_{},
      window_{cfg_->eng.resolution, cfg_->GetWindowName(), user_ptr},
      engine_{cfg_, &window_} {}
