#ifndef JOYPAD_HPP
#define JOYPAD_HPP

#include <SDL2/SDL.h>
#include <cstdint>
#include "interrupt_manager.hpp"

/* Button
 * Enum mapping the 8 GameBoy buttons to keyboard keys. */
enum class Button : int {
    START = SDL_SCANCODE_RETURN,
    SELECT = SDL_SCANCODE_BACKSPACE,
    B = SDL_SCANCODE_Z,
    A = SDL_SCANCODE_X,
    DOWN = SDL_SCANCODE_DOWN,
    UP = SDL_SCANCODE_UP,
    LEFT = SDL_SCANCODE_LEFT,
    RIGHT = SDL_SCANCODE_RIGHT
};

/* Joypad 
 * Converts keyboard key presses into the JOYP register for the MMU to read. */
class Joypad {
public:
    Joypad(InterruptManager* interrupt_manager) :
        interrupt_manager(interrupt_manager) { reset() ;}

    void reset() { joyp_ = 0xCF; }
    uint8_t joyp();
    void set_joyp(uint8_t value) {
        joyp_ = (joyp_ & 0b11001111) | (value & 0b00110000);
    };

private:
    InterruptManager* interrupt_manager;

    /* 0xFF00 register:
    * - bits 7-6 - unused (always 1)
    * - bit 5 - action buttons (read/write)
    * - bit 4 - direction buttons (read/write)
    * - bit 3 - start or down (read only)
    * - bit 2 - select or up (read only)
    * - bit 1 - b or left (read only)
    * - bit 0 - a or right (read only)
    * In all cases 0 = pressed, 1 = unpressed. */
    uint8_t joyp_;
};

#endif