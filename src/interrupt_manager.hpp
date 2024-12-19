#ifndef INTERRUPT_MANAGER_HPP
#define INTERRUPT_MANAGER_HPP

#include <cstdint>

// Handles enabling and disabling of hardware interrupts.
class InterruptManager {
public:
    InterruptManager() {reset();};
    ~InterruptManager() {};

    bool ime;    // Interrupt Master Enable
    uint8_t ie;  // Interrupt Enable Register
    uint8_t ix;  // Interrupt Flag Register

    /* There are 5 types of interrupt, whose values correspond to their bit
     * position in ie and ix, as well as the index in the handler_addresses
     * array. */
    enum InterruptType {
        Vblank = 0,
        Lcd = 1,
        Timer = 2,
        Serial = 3,
        Joypad = 4
    };

    void reset();
    void request_interrupt(InterruptType interruption);
    bool is_interrupt_requested()const;
    int get_enabled_interrupt() const;

private:
    // Addresses to set reg.pc to for each corresponding interrupt
    const uint8_t handler_addresses[5] = {0x40, 0x48, 0x50, 0x58, 0x60};
};

#endif