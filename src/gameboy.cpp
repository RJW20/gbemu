#include <chrono>
#include <thread>
#include <SDL2/SDL.h>
#include "gameboy.hpp"

void GameBoy::run() {

    using namespace std::chrono;
    using time_between_frames = duration<int64_t, std::ratio<1, 60>>;
    auto next_frame = system_clock::now() + time_between_frames{0};
    auto last_frame = next_frame - time_between_frames{1};

    while (true) {
        tick();
        if (++ticks == SIXTY_FPS) {
            screen.render();
            ticks = 0;
            if (power_off()) {
                break;
            }
            std::this_thread::sleep_until(next_frame);
            last_frame = next_frame;
            next_frame = next_frame + time_between_frames{1};
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