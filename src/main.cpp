#include "utils/include_glfw.hpp"

#include "game/game.hpp"

int main(void) {

  glfwInit();

  auto game = Game();
  game.Run();

  glfwTerminate();

  return 0;
}
