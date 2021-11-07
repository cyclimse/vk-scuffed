#include "sc_game.hpp"

#include <memory>

using namespace sc;

Game::Game()
    : cfg_{std::make_shared<Config>()}, renderer_{cfg_, this}, keys_pressed_{false} {}

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
