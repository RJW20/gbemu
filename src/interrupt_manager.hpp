#ifndef INTERRUPT_MANAGER_HPP
#define INTERRUPT_MANAGER_HPP

#include <cstdint>

class InterruptManager {
public:
    bool ime;    // Interrupt Master Enable
    uint8_t ie;  // Interrupt Enable Register
    uint8_t ix;  // Interrupt Flag Register

    InterruptManager();
    ~InterruptManager() {};

    void request_interrupt(uint8_t bit);
    void disable_interrupts();
    void enable_interrupts();

private:
};

#endif