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

    void request_interrupt(uint8_t interrupt_bit);

    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);

private:
};

#endif