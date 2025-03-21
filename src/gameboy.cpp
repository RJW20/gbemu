#include <chrono>
#include <thread>
#include <SDL2/SDL.h>
#include "gameboy.hpp"

void GameBoy::run() {

    using namespace std::chrono;
    using time_between_frames = duration<int64_t, std::ratio<1, FPS>>;
    auto next_frame = system_clock::now() + time_between_frames{0};
    auto last_frame = next_frame - time_between_frames{1};

    while (true) {

        tick();

        if (!ppu.ready_to_render) {
            continue;
        }

        screen.render();

        if (power_off()) {
            break;
        }
        
        if (throttled()) {
            std::this_thread::sleep_until(next_frame);
            last_frame = next_frame;
            next_frame = next_frame + time_between_frames{1};
        }
        else {
            next_frame = system_clock::now() + time_between_frames{0};
        }
    }
}

// Carry out 1 t-cycle.
void GameBoy::tick() {
    timer.tick();
    serial.tick();
    ppu.tick();
    dma.tick();
    cpu.tick();
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

// Return true if the SPACE key is not currently pressed.
bool GameBoy::throttled() const {
    const uint8_t* keyboard = SDL_GetKeyboardState(NULL);
    return !keyboard[SDL_SCANCODE_SPACE];
}