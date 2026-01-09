#include "engine/OpenGL.h"
#include "game/DungeonsGame.h"
#include <iostream>

int main(int argc, char *argv[]) {
  try {
    DungeonsGame game;
    game.Run();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}