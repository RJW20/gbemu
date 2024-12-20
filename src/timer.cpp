#include <cstdint>
#include "timer.hpp"
#include "interrupt_manager.hpp"

// Initialise all components to zero.
void Timer::reset() {
    system_counter = 0;
    tima = 0;
    tma = 0;
    tac = 0;
    previous_sc_bit = 0;
}

// Carry out 1 t-cycle.
void Timer::tick() {

    system_counter += 1;

    if (!timer_is_enabled()) {
        return;
    }

    // Increment tima based on the system_counter bit specified in tac
    bool current_sc_bit = (system_counter >> tac_bit_select[tac & 0x03]) & 1;
    if (previous_sc_bit && !current_sc_bit) {   // falling edge
        if (++tima == 0) {
            tima = tma;
            interrupt_manager->request(InterruptType(2));
        }
    }
    previous_sc_bit = current_sc_bit;
}

// Return the upper 8 bits of the system_counter.
uint8_t Timer::div() const {
    return system_counter >> 8;
}

// Set the system_counter to zero.
void Timer::set_div() {
    system_counter = 0;
}

// Set the timer control to the lower 3 bits of the given value.
void Timer::set_tac(uint8_t value) {
    tac = value & 0x07;
}

// Check if the timer is enabled.
bool Timer::timer_is_enabled() const {
    return tac & 0x04; // Bit 2 of tac
}