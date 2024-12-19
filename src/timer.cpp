#include <cstdint>
#include "timer.hpp"
#include "interrupt_manager.hpp"

// Initialise all components to zero.
void Timer::reset() {
    div = 0;
    tima = 0;
    tma = 0;
    tac = 0;

    div_counter = 0;
    tima_counter = 0;
}

// Carry out 1 t-cycle.
void Timer::tick() {

    // Increment div every 256 = 2^8 t-cycles
    div_counter += 1;
    div += (div_counter >> 8);
    div_counter &= 0xFF;

    // Increment tima at the frequency specified in tac
    if (timer_is_enabled()) {
        tima_counter += 1;

        if (tima_counter == tima_t_cycles[tac & 0x03]) {
            tima_counter = 0;
            
            if (++tima == 0) {
                tima = tma;
                interrupt_manager->request(InterruptType(2));
            }
            
        }
    }
}

// Return the upper 8 bits of the divider register.
uint8_t Timer::exposed_div() const {
    return div >> 8;
}

// Set the divider register to zero.
void Timer::reset_div() {
    div = 0;
}

// Set the timer control to the lower 3 bits of the given value.
void Timer::set_tac(uint8_t value) {
    tac = value & 0x07;
}

// Check if the timer is enabled.
bool Timer::timer_is_enabled() const {
    return tac & 0x04; // Bit 2 of tac
}