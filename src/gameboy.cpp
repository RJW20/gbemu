#include <sstream>
#include <SDL2/SDL.h>
#include "logger.hpp"
#include "gameboy.hpp"

void GameBoy::run() {
    while (true) {
        tick();
        if (++ticks == SIXTY_FPS) {
            screen.render();
            ticks = 0;
            if (power_off()) {
                break;
            }
        }
    }
}

// Carry out 1 t-cycle.
void GameBoy::tick() {
    timer.tick();
    cpu.tick();
    serial.tick();
    dma.tick();
    ppu.tick();

    Log::debug(*this);
}

// Return true if an SDL_QUIT event has occurred.
bool GameBoy::power_off() const {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return true;
        }
    }
    return false;
}

// Return a string representation of the GameBoy.
std::string GameBoy::representation() const {
    std::ostringstream repr;
    repr << "GameBoy: " << cpu;
    return repr.str();
}