#include <cstdint>
#include <string>
#include <algorithm>
#include <SDL2/SDL.h>
#include "logger.hpp"
#include "screen.hpp"
#include "ppu/ppu.hpp"

// Initialise the SDL2 components.
Screen::Screen(const std::string rom_title, Ppu* ppu) : ppu(ppu) {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Log::error(
            "SDL could not initialize! SDL_Error: " +
            std::string(SDL_GetError())
        );
        exit(4);
    };

    window = SDL_CreateWindow(
        rom_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        ppu->SCREEN_WIDTH * 4, ppu->SCREEN_HEIGHT * 4, 0
    );
    if (!window) {
        Log::error(
            "Window could not be created! SDL_Error: " +
            std::string(SDL_GetError())
        );
        exit(5);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        Log::error(
            "Renderer could not be created! SDL_Error: " +
            std::string(SDL_GetError())
        );
        exit(6);
    }

    texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
        ppu->SCREEN_WIDTH, ppu->SCREEN_HEIGHT
    );
    if (!texture) {
        Log::error(
            "Texture could not be created! SDL_Error: " +
            std::string(SDL_GetError())
        );
        exit(7);
    }
}

// Destroy all SDL components.
Screen::~Screen() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/* Draw the PPU's pixel_buffer to the window.
 * Should be called every time the pixel_buffer is updated, but the end of
 * each PPU VBLANK (every 70224 ticks = ~60FPS) is sufficient and efficient. */
void Screen::render() {

    void* pixel_pointer;
    int pitch;
    if (SDL_LockTexture(texture, nullptr, &pixel_pointer, &pitch) != 0) {
        Log::error(
            "SDL_LockTexture failed! SDL_Error: " + std::string(SDL_GetError())
        );
        exit(8);
    }

    std::copy(
        ppu->pixel_buffer.begin(),
        ppu->pixel_buffer.end(),
        static_cast<uint32_t*>(pixel_pointer)
    );

    SDL_UnlockTexture(texture);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    ppu->ready_to_render = false;
}