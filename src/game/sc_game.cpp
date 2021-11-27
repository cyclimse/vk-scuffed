#include "sc_game.hpp"

#include <memory>

#include "sc_assets.hpp"

using namespace sc;

Game::Game()
    : json_parser_{},
      cfg_{std::make_unique<Config>(Config::Load(json_parser_, "config.json"))},
      assets_{std::make_unique<Assets>(Assets::Load(json_parser_, "assets"))},
      renderer_{cfg_.get(), assets_.get(), this},
      keys_pressed_{false} {}

void Game::Run() {
  while (!renderer_.window_.ShouldClose()) {
    glfwPollEvents();
    this->Update();
  }
}

void Game::Update() {
  if (keys_pressed_.at(GLFW_KEY_ESCAPE)) {
    renderer_.window_.Close();
  }
}
