#include "timer.hpp"

// Initialise all components to zero.
Timer::Timer() {
    div = 0;
    tima = 0;
    tma = 0;
    tac = 0;

    div_counter = 0;
    tima_counter = 0;
}

/*
 */
void Timer::tick() {

    // Increment div every 64 = 2^6 m-cycles
    div_counter += 1;
    div += (div_counter >> 6);
    div_counter &= 0x3F;

    // Increment tima at the frequency specified in tac
    if ((tac >> 2) & 1) {
        tima_counter += 1;

        uint8_t m_cycles_exponent = m_cycles_exponents[tac & 0b11];
        if ((tima_counter >> m_cycles_exponent) & 1) {
            if (tima & 0xFF) {
                tma += 1;
            }
            else {
                tima = tma;
                // TODO: send interrupt here
            }
            tima_counter &= (1 << m_cycles_exponent) - 1;
        }
    }
}