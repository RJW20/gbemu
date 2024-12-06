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

// Read from an interrupt register address.
uint8_t InterruptManager::read(uint16_t address) const {
    switch (address) {
        case 0xFFFF:
            return ie;
        case 0xFF0F:
            return ix;
        default:
            std::cerr << "Invalid interrupt register read at address: " <<
                std::hex << address << std::endl;
            return 0xFF;
    }
}

// Write to an interrupt register address.
void InterruptManager::write(uint16_t address, uint8_t value) {
    switch (address) {
        case 0xFFFF:
            ie = value;
        case 0xFF0F:
            ix = value;
        default:
            std::cerr << "Invalid interrupt register write at address: " <<
                std::hex << address << std::endl;
    }
}