#include <iostream>
#include <cstdint>
#include <utility>
#include <SDL2/SDL.h>
#include "joypad.hpp"
#include "interrupt_manager.hpp"

/* Set joyp_ to the value determined by it's own bits 5-4 and the state of the
 * keyboard keys representing the GameBoy buttons.
 * Returns 0xFF if both bits 5 and 4 are set. */
uint8_t Joypad::joyp() {

    // If neither button type is selected joyp_ is not updated and 0xFF is read
    const bool action_buttons = !(joyp_ & 0x20);
    const bool direction_buttons = !(joyp_ & 0x10);
    if (!action_buttons & !direction_buttons) {
        return 0xFF;
    }

    const uint8_t previous_joyp = joyp_;

    const uint8_t* keyboard = SDL_GetKeyboardState(NULL);
    uint8_t buttons = 0xF;
    if (action_buttons) {
        buttons &= (
            (!keyboard[std::to_underlying(Button::START)] << 3) |
            (!keyboard[std::to_underlying(Button::SELECT)] << 2) |
            (!keyboard[std::to_underlying(Button::B)] << 1) |
            (!keyboard[std::to_underlying(Button::A)])
        );
    }
    if (direction_buttons) {
        buttons &= (
            (!keyboard[std::to_underlying(Button::DOWN)] << 3) |
            (!keyboard[std::to_underlying(Button::UP)] << 2) |
            (!keyboard[std::to_underlying(Button::LEFT)] << 1) |
            (!keyboard[std::to_underlying(Button::RIGHT)])
        );
    }
    joyp_ = (joyp_ & 0xF0) | buttons;

    // Request interrupt if a button bit went from unpressed to pressed
    if ((previous_joyp & 0xF) & ~(joyp_ & 0xF)) {
        interrupt_manager->request(InterruptType::JOYPAD);
    }

    return joyp_;
}