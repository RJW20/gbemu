#include <chrono>
#include <thread>
#include <utility>
#include <SDL2/SDL.h>
#include "gameboy.hpp"
#include "joypad.hpp"

void GameBoy::run() {

    using namespace std::chrono;
    using time_between_frames = duration<int64_t, std::ratio<1, FPS>>;
    auto next_frame = system_clock::now() + time_between_frames{0};

    while (!power_off) {

        tick();

        if (!ppu.ready_to_render) {
            continue;
        }
        screen.render();

        check_events();
        if (power_off) {
            break;
        }
        if (reset_) {
            reset();
            continue;;
        }
        if (throttled) {
            std::this_thread::sleep_until(next_frame);
            next_frame = next_frame + time_between_frames{1};
        }
        else {
            next_frame = system_clock::now() + time_between_frames{0};
        }
        while (paused) {
            check_events();
            if (power_off) {
                break;
            }
            if (reset_) {
                reset();
                paused = false;
                break;
            }
            std::this_thread::sleep_until(next_frame);
            next_frame = system_clock::now() + time_between_frames{0};
        }
    }
}

// Set all components to their post boot ROM state.
void GameBoy::reset() {
    cartridge.reset();
    interrupt_manager.reset();
    timer.reset();
    serial.reset();
    joypad.reset();
    ppu.reset();
    dma.reset();
    apu.reset();
    mmu.reset();
    cpu.reset();
    reset_ = false;
}

// Carry out 1 t-cycle.
void GameBoy::tick() {
    timer.tick();
    serial.tick();
    ppu.tick();
    dma.tick();
    cpu.tick();
}

/* Check SDL events.
 * Sets power_off if an SDL_QUIT event has occured.
 * Flips paused if the P key has been pressed.
 * Sets reset_ if the START, SELECT, A and B buttons are pressed (a soft reset).
 * Resets throttled if the SPACE key is currently pressed. */
void GameBoy::check_events() {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            power_off = true;
        }
        else if (event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == SDLK_p) {
            paused = !paused;
        }
    }

    const uint8_t* keyboard = SDL_GetKeyboardState(NULL);
    reset_ = keyboard[std::to_underlying(Button::START)] &&
        keyboard[std::to_underlying(Button::SELECT)] && 
        keyboard[std::to_underlying(Button::A)] &&
        keyboard[std::to_underlying(Button::B)];
    throttled = !keyboard[SDL_SCANCODE_SPACE];
}