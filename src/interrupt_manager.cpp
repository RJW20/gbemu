#include <iostream>
#include <cstdint>
#include "interrupt_manager.hpp"

// Initialise all components to zero.
InterruptManager::InterruptManager() {
    ime = false;
    ie = 0;
    ix = 0;
}

/* Request an interrupt of the given type.
 * The value of the given bit decides the type of interrupt:
 * - 0   Vblank 
 * - 1   LCD
 * - 2   Timer 
 * - 3   Serial Link 
 * - 4   Joypad */
void InterruptManager::request_interrupt(uint8_t bit) {
    ix |= (1 << bit);
}

/* Set ime to false. */
void InterruptManager::disable_interrupts() {
    ime = false;
}

/* Set ime to true. */
void InterruptManager::enable_interrupts() {
    ime = true;
}