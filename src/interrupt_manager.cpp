#include <cstdint>
#include "interrupt_manager.hpp"

// Initialise all components to zero.
InterruptManager::InterruptManager() {
    ime = false;
    ie = 0;
    ix = 0;
}

/* Request an interrupt of the given type.
 * The value of the given bit decides the type of interrupt:
 * - 0   Vblank 
 * - 1   LCD
 * - 2   Timer 
 * - 3   Serial Link 
 * - 4   Joypad */
void InterruptManager::request_interrupt(uint8_t bit) {
    ix |= (1 << bit);
}

/* Return true if interrupts are enabled and a specific interrupt is
 * requested. */
bool InterruptManager::is_interrupt_requested() {
    return ime && (ie & ix);
}

/* Return the bit corresponding to the enabled interrupt.
 * Interrupts with lower bit value with have higher priority.
 * Returns -1 if no interrupts are enabled. */
int InterruptManager::get_enabled_interrupt() {
    int interrupt_bit;
    for (int bit_pos = 0< bit_pos < 5; bit_pos++;) {
        if (((ie & ix) >> bit_pos) & 1 == 1) {
            return interrupt_bit;
        }
    }
    return -1;
}