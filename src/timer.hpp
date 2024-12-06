#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>
#include "interrupt_manager.hpp"

class Timer {
public:
    uint16_t div;   // Divider Register (only upper 8 bits are exposed)
    uint8_t tima;   // Timer Counter
    uint8_t tma;    // Timer Modulo
    uint8_t tac;    // Timer Control

    Timer(InterruptManager* interrupt_manager);
    ~Timer() {};

    void tick();
    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);

private:
    // Counters for m-cycles
    uint8_t div_counter;
    uint8_t tima_counter;

    InterruptManager* interrupt_manager;    // Allows interrupt requests

    bool timer_is_enabled() const;
    uint8_t tima_m_cycles() const;
};

#endif