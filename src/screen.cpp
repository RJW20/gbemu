#include <iostream>
#include <string>
#include <cstdint>
#include <SDL2/SDL.h>
#include "screen.hpp"
#include "ppu.hpp"

// Initialise the SDL2 components.
Screen::Screen(const std::string rom_title, Ppu* ppu) : ppu(ppu) {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
            << std::endl;
        exit(1);
    };

    window = SDL_CreateWindow(
        rom_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        ppu->SCREEN_WIDTH, ppu->SCREEN_HEIGHT, 0
    );
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: "
            << SDL_GetError() << std::endl;
        exit(2);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: "
            << SDL_GetError() << std::endl;
        exit(3);
    }

    texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
        ppu->SCREEN_WIDTH, ppu->SCREEN_HEIGHT
    );
    if (!texture) {
        std::cerr << "Texture could not be created! SDL_Error: "
            << SDL_GetError() << std::endl;
        exit(4);
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
 * Should be called every time the pixel_buffer is updated, but ~60FPS (every 
 * 70224 ticks) is sufficient and efficient. */
void Screen::render() {

    void* pixel_pointer;
    int pitch;
    if (SDL_LockTexture(texture, nullptr, &pixel_pointer, &pitch) != 0) {
        std::cerr << "SDL_LockTexture failed: " << SDL_GetError() << std::endl;
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
}