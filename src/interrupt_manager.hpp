#ifndef INTERRUPT_MANAGER_HPP
#define INTERRUPT_MANAGER_HPP

#include <cstdint>
#include <array>

/* There are 5 types of interrupt, whose values correspond to their bit
 * position in ie and ix, as well as the index in the handler_addresses
 * array:
 * - 0 - Vblank 
 * - 1 - STAT
 * - 2 - Timer 
 * - 3 - Serial Link 
 * - 4 - Joypad */
enum class InterruptType : int {
    VBLANK = 0,
    STAT = 1,
    TIMER = 2,
    SERIAL = 3,
    JOYPAD = 4,
    NONE = -1
};

// Handles enabling and disabling of hardware interrupts.
class InterruptManager {
public:
    InterruptManager() { reset(); };
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
    static constexpr std::array<uint8_t, 5> handler_addresses = {
        0x40, 0x48, 0x50, 0x58, 0x60
    };
};

#endif