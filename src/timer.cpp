#include <cstdint>
#include <string>
#include <sstream>
#include <bitset>
#include "logger.hpp"
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

    const bool current_sc_bit =
        (system_counter >> tac_clock_select.at(tac_ & 3)) & 1;
    if (timer_is_enabled() && previous_sc_bit && !current_sc_bit) {
        increase_tima();
    }
    previous_sc_bit = current_sc_bit;

    // Send interrupt request if tima_ overflowed last m-cycle
    if (tima_overflow && ++ticks_since_overflow == 4) {
        tima_ = tma_;
        interrupt_manager->request(InterruptType::TIMER);
        tima_overflow = false;
    }

    Log::debug(*this);
}

// Increment tima_ and check for an overflow.
void Timer::increase_tima() {
    if (++tima_ == 0) {
        tima_overflow = true;
        ticks_since_overflow = 0;
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

/* Set tac_ to the trailing 3 bits in the given value.
 * If this write disables the timer and the specified system_counter bit is
 * set, tima_ will increase.
 * If the specified system_counter bit changes then we need to check for
 * falling edge between the old bit and new bit and update tima_
 * accordingly. */
void Timer::set_tac(uint8_t value) {

    const bool previous_timer_enabled = timer_is_enabled();
    const uint8_t previous_clock_select = tac_ & 3;

    tac_ = value & 7;

    if (previous_timer_enabled && !timer_is_enabled() && previous_sc_bit) {
        increase_tima();
    }
    else if (previous_clock_select != (tac_ & 3)) {
        const bool current_sc_bit =
            (system_counter >> tac_clock_select.at(tac_ & 3)) & 1;
        if (previous_sc_bit && !current_sc_bit) {
            increase_tima();
        }
        previous_sc_bit = current_sc_bit;
    }
}

// Return a string representation of the Timer.
std::string Timer::representation() const {
    std::ostringstream repr;
    repr << "Timer:" << std::hex
        << " System Counter = " << std::bitset<16>(system_counter)
        << " TIMA = " << static_cast<int>(tima())
        << " TMA = " << static_cast<int>(tma())
        << " TAC = " << static_cast<int>(tac());
    return repr.str();
}