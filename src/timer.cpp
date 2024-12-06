#include <iostream>
#include <cstdint>
#include "timer.hpp"
#include "interrupt_manager.hpp"

// Initialise all components to zero and set the interrupt manager pointer.
Timer::Timer(InterruptManager* inter_manag) {
    div = 0;
    tima = 0;
    tma = 0;
    tac = 0;

    div_counter = 0;
    tima_counter = 0;

    interrupt_manager = inter_manag;
}

// Update the timer (called every m-cycle).
void Timer::tick() {

    // Increment div every 64 = 2^6 m-cycles
    div_counter += 1;
    div += (div_counter >> 6);
    div_counter &= 0x3F;

    // Increment tima at the frequency specified in tac
    if (timer_is_enabled()) {
        tima_counter += 1;

        if (tima_counter >= tima_m_cycles()) {
            tima_counter = 0;
            
            if (++tima == 0) {
                tima = tma;
                interrupt_manager->request_interrupt(2);
            }
            
        }
    }
}

// Read from a timer memory address.
uint8_t Timer::read(uint16_t address) const {
    switch (address) {
        case 0xFF04:
            return div >> 8; // Upper 8 bits of div
        case 0xFF05:
            return tima;
        case 0xFF06:
            return tma;
        case 0xFF07:
            return tac;
        default:
            std::cerr << "Invalid timer read at address: " << std::hex <<
                address << std::endl;
            return 0xFF;
    }
}

// Write to a timer memory address.
void Timer::write(uint16_t address, uint8_t value) {
    switch (address) {
        case 0xFF04:
            div = 0; // Writing to div resets it
            break;
        case 0xFF05:
            tima = value;
            break;
        case 0xFF06:
            tma = value;
            break;
        case 0xFF07:
            tac = value & 0x07; // Only the lower 3 bits are valid
            break;
        default:
            std::cerr << "Invalid timer write at address: " << std::hex <<
                address << std::endl;
    }
}

// Check if the timer is enabled.
bool Timer::timer_is_enabled() const {
    return tac & 0x04; // Bit 2 of tac
}

// Return the number of m-cycles per tima tick.
uint8_t Timer::tima_m_cycles() const {
    switch (tac & 0x03) { // Bits 0–1 of tac
        case 0x00:
            return (uint8_t) 0xFF; // 4096 Hz
        case 0x01:
            return (uint8_t) 0x4; // 262144 Hz
        case 0x10:
            return (uint8_t) 0x10; // 65536 Hz
        case 0x11:
            return (uint8_t) 0x40; // 16384 Hz
        default:
            return 0xFF;    // Never reach here
    }
}