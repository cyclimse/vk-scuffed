#include <GLFW/glfw3.h>

#include "sc_game.hpp"

int main(void) {
  auto game = sc::Game{};

  game.Run();

  return 0;
}
