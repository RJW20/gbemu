#include <string>
#include <SDL2/SDL.h>
#include "logger.hpp"
#include "gameboy.hpp"

int main(int argc, char** argv) {

    // Check for rom provided
    if (argc != 2) {
        Log::info("Usage: gbemu <ROM file>");
    }

    GameBoy gameboy(static_cast<std::string>(argv[1]));
    gameboy.run();

    return 0;
}