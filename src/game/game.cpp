#include "game.hpp"

Game::Game() : window_{this}, engine_{&window_}, keys_pressed_{false} {}

void Game::Run() {
  while (!window_.ShouldClose()) {
    glfwPollEvents();
    this->Update();
  }
}

void Game::Update() {
  if (keys_pressed_.at(GLFW_KEY_ESCAPE)) {
    window_.Close();
  }
}
