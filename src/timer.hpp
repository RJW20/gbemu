#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>

class Timer {
public:
    Timer();
    ~Timer() {};

    void tick();

    uint8_t div;    // Divider Register
    uint8_t tima;   // Timer Counter
    uint8_t tma;    // Timer Modulo
    uint8_t tac;    // Timer Control

private:
    // Counters for m-cycles
    uint8_t div_counter;
    uint8_t tima_counter;

    // Array linking bits in tac to the frequency to update tima
    const uint8_t m_cycles_exponents[4] = {8, 2, 4, 6};
};

#endif