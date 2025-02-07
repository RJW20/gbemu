#ifndef INTERRUPT_MANAGER_HPP
#define INTERRUPT_MANAGER_HPP

#include <cstdint>
#include <array>

/* There are 5 types of interrupt, whose values correspond to their bit
 * position in ie_ and if_, as well as the index in the handler_addresses
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
    InterruptManager() { reset(); }
    void reset();

    void request(InterruptType interruption);
    void acknowledge(InterruptType interruption);
    bool is_interrupt_requested()const;
    InterruptType get_enabled() const;
    uint8_t get_handler_address(InterruptType interruption);

    void disable_interrupts() { ime = false; }
    void enable_interrupts() { ime = true; }
    uint8_t ie() const { return ie_; }
    uint8_t _if() const { return if_; }
    void set_ie(uint8_t value) { ie_ = value & 0x1F; }
    void set_if(uint8_t value) { if_ = value & 0x1F; }

private:
    bool ime;    // Interrupt master enable
    uint8_t ie_;  // Interrupt enable register
    uint8_t if_;  // Interrupt flag register

    // Addresses to set reg.pc to for each corresponding interrupt
    static constexpr std::array<uint8_t, 5> handler_addresses = {
        0x40, 0x48, 0x50, 0x58, 0x60
    };
};

#endif