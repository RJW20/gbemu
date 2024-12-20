#include <cstdint>
#include "timer.hpp"
#include "interrupt_manager.hpp"

// Reset all components to zero.
void Timer::reset() {
    system_counter = 0;
    tima = 0;
    tma = 0;
    tac = 0;
    previous_sc_bit = 0;
    tima_overflow = false;
    ticks_since_overflow = 0;
}

// Carry out 1 t-cycle.
void Timer::tick() {

    system_counter += 1;

    if (!timer_is_enabled()) {
        return;
    }

    // Increment tima based on the system_counter bit specified in tac
    bool current_sc_bit = (system_counter >> tac_clock_select[tac & 0x03]) & 1;
    if (previous_sc_bit && !current_sc_bit) {   // falling edge
        if (++tima == 0) {
            tima_overflow = true;
            ticks_since_overflow = -1;
        }
    }
    previous_sc_bit = current_sc_bit;

    // Send interrupt request if tima overflowed last cycle
    if (tima_overflow) {
        ticks_since_overflow += 1;
        if (ticks_since_overflow == 4) {
            tima = tma;
            interrupt_manager->request(InterruptType::Timer_);
            tima_overflow = false;
        }
    }
}

// Return the upper 8 bits of the system_counter.
uint8_t Timer::div() const {
    return system_counter >> 8;
}

// Set the system_counter to zero.
void Timer::set_div() {
    system_counter = 0;
}

/* Set tima to the given value.
 * If a tima overflow occurred in the previous cycle the write will be ignored.
 * If a tima overflow occurred in this cycle the overflow will be ignored. */
void Timer::set_tima(uint8_t value) {
    if (ticks_since_overflow == 4) {
        return;
    }
    if (tima_overflow) {
        tima_overflow = false;
    }
    tima = value;
}

/* Set tma to the given value.
 * If a tima overflow occurred in the previous cycle then tima will also be set
 * to the same value. */
void Timer::set_tma(uint8_t value) {
    tma = value;
    if (ticks_since_overflow == 4) {
    tima = value;
    }
}

// Set the timer control to the lower 3 bits of the given value.
void Timer::set_tac(uint8_t value) {
    tac = value & 0x07;
}

// Check if the timer is enabled.
bool Timer::timer_is_enabled() const {
    return tac & 0x04; // Bit 2 of tac
}