#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <string>
#include <SDL2/SDL.h>
#include "ppu/ppu.hpp"

/* Screen 
 * Manages all SDL2 components.
 * Draws the pixels outputted by the PPU. */
class Screen {
public:
    Screen(const std::string rom_title, Ppu* ppu);
    ~Screen();

    void render();
    
private:
    Ppu* ppu;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

#endif