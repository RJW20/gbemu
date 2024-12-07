#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>
#include "interrupt_manager.hpp"

class Timer {
public:
    uint8_t tima;   // Timer Counter
    uint8_t tma;    // Timer Modulo
    uint8_t tac;    // Timer Control

    Timer(InterruptManager* interrupt_manager);
    ~Timer() {};

    void tick();
    uint8_t exposed_div() const;
    void reset_div();
    void set_tac(uint8_t value);

private:
    uint16_t div;   // Divider Register (only upper 8 bits are exposed)

    // Counters for m-cycles
    uint8_t div_counter;
    uint8_t tima_counter;

    InterruptManager* interrupt_manager;    // Allows interrupt requests

    bool timer_is_enabled() const;
    uint8_t tima_m_cycles() const;
};

#endif