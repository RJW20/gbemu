#ifndef INTERRUPT_MANAGER_HPP
#define INTERRUPT_MANAGER_HPP

#include <cstdint>

/* There are 5 types of interrupt, whose values correspond to their bit
 * position in ie and ix, as well as the index in the handler_addresses
 * array:
 * - 0 - Vblank 
 * - 1 - LCD
 * - 2 - Timer 
 * - 3 - Serial Link 
 * - 4 - Joypad */
enum InterruptType {
    Vblank = 0,
    Lcd = 1,
    Timer_ = 2,
    Serial_ = 3,
    Joypad = 4,
    None = -1
};

// Handles enabling and disabling of hardware interrupts.
class InterruptManager {
public:
    InterruptManager() {reset();};
    ~InterruptManager() {};

    bool ime;    // Interrupt Master Enable
    uint8_t ie;  // Interrupt Enable Register
    uint8_t ix;  // Interrupt Flag Register

    void reset();
    void request(InterruptType interruption);
    void acknowledge(InterruptType interruption);
    bool is_interrupt_requested()const;
    InterruptType get_enabled() const;
    uint8_t get_handler_address(InterruptType interruption);

private:
    // Addresses to set reg.pc to for each corresponding interrupt
    const uint8_t handler_addresses[5] = {0x40, 0x48, 0x50, 0x58, 0x60};
};

#endif