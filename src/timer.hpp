#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>
#include "interrupt_manager.hpp"

/* Simulates the system's timer hardware by managing clock cycles, updating
 * timer registers, and triggering interrupts at specified intervals. */
class Timer {
public:
    Timer(InterruptManager* interrupt_manager) :
        interrupt_manager(interrupt_manager) {reset();};
    ~Timer() {};

    uint8_t tima;   // Timer Counter
    uint8_t tma;    // Timer Modulo
    uint8_t tac;    // Timer Control

    void reset();
    void tick();
    uint8_t exposed_div() const;
    void reset_div();
    void set_tac(uint8_t value);

private:
    InterruptManager* interrupt_manager;

    uint16_t div;   // Divider Register (only upper 8 bits are exposed)

    // Counters for t-cycles
    uint16_t div_counter;
    uint16_t tima_counter;

    /* The number of t-cycles per tima tick dependent on the trailing 2 bits of
     * tac (used as index of the array):
     * - 00 - 1024 (4096 Hz)
     * - 01 - 16 (262144 Hz)
     * - 10 - 64 (65536 Hz)
     * - 11 - 256 (16384 Hz) */
    const uint16_t tima_t_cycles[4] = {0x400, 0x10, 0x40, 0x100};

    bool timer_is_enabled() const;
};

#endif