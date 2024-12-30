#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "gameboy.hpp"

int main(int argc, char** argv) {

    // Check for rom provided
    if (argc != 2) {
        std::cout << "Usage: gbemu <ROM file>" << std::endl;
        return 1;
    }

    GameBoy gameboy((std::string) argv[1]);
    gameboy.run();

    return 0;
}