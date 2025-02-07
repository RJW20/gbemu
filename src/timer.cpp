#include <cstdint>
#include "timer.hpp"
#include "interrupt_manager.hpp"

// Set the registers to their post boot ROM values.
void Timer::reset() {
    system_counter = 0;
    tima_ = 0;
    tma_ = 0;
    tac_ = 0xF8;
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

    // Increment tima_ based on the system_counter bit specified in tac_
    const bool current_sc_bit =
        (system_counter >> tac_clock_select.at(tac_ & 0x3)) & 1;
    if (previous_sc_bit && !current_sc_bit) {   // falling edge
        if (++tima_ == 0) {
            tima_overflow = true;
            ticks_since_overflow = 0;
        }
    }
    previous_sc_bit = current_sc_bit;

    // Send interrupt request if tima_ overflowed last cycle
    if (tima_overflow && ticks_since_overflow++ == 4) {
        tima_ = tma_;
        interrupt_manager->request(InterruptType::TIMER);
        tima_overflow = false;
    }
}

/* Set tima_ to the given value.
 * If a tima_ overflow occurred in the previous cycle the write will be
 * ignored.
 * If a tima_ overflow occurred in this cycle the overflow will be ignored. */
void Timer::set_tima(uint8_t value) {
    if (ticks_since_overflow == 4) {
        return;
    }
    if (tima_overflow) {
        tima_overflow = false;
    }
    tima_ = value;
}

/* Set tma_ to the given value.
 * If a tima_ overflow occurred in the previous cycle then tima_ will also be
 * set to the same value. */
void Timer::set_tma(uint8_t value) {
    tma_ = value;
    if (ticks_since_overflow == 4) {
        tima_ = value;
    }
}