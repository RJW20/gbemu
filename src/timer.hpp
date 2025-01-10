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

    void reset();
    void tick();
    uint8_t div() const;
    uint8_t tima() const;
    uint8_t tma() const;
    uint8_t tac() const;
    void set_div();
    void set_tima(uint8_t value);
    void set_tma(uint8_t value);
    void set_tac(uint8_t value);

private:
    InterruptManager* interrupt_manager;

    uint16_t system_counter;    // Divider register exposes upper 8 bits
    bool previous_sc_bit;   // Previous value of selected bit in system_counter

    uint8_t tima_;   // Timer counter
    uint8_t tma_;    // Timer modulo
    uint8_t tac_;    // Timer control

    /* tima is updated based on the system_counter bit transitions - the bit
     * that is monitored is dependent on the trailing 2 bits of tac (used as
     * index of the array):
     * - 00 - 9 (4096 Hz)
     * - 01 - 3 (262144 Hz)
     * - 10 - 5 (65536 Hz)
     * - 11 - 7 (16384 Hz) */
    const uint8_t tac_clock_select[4] = {9, 3, 5, 7};

    // Variables to ensure overflow behaviour is correct
    bool tima_overflow;
    int ticks_since_overflow;

    bool timer_is_enabled() const;
};

#endif