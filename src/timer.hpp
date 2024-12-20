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
    uint8_t div() const;    // Divider Register
    void set_div();
    void set_tac(uint8_t value);

private:
    InterruptManager* interrupt_manager;

    uint16_t system_counter;    // Divider register exposes upper 8 bits
    bool previous_sc_bit; // Previous value of selected bit in system_counter

    /* tima is updated based on the system_counter bit transitions - the bit
     * that is monitored is dependent on the trailing 2 bits of tac (used as
     * index of the array):
     * - 00 - 1024 (4096 Hz)
     * - 01 - 16 (262144 Hz)
     * - 10 - 64 (65536 Hz)
     * - 11 - 256 (16384 Hz) */
    const uint8_t tac_bit_select[4] = {9, 3, 5, 7};

    bool timer_is_enabled() const;
};

#endif