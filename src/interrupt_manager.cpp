#include "interrupt_manager.hpp"

// Reset all attributes.
void InterruptManager::reset() {
    ime = false;
    ie = 0;
    ix = 0;
}

// Request an interrupt of the given type.
void InterruptManager::request_interrupt(InterruptType interruption) {
    ix |= (1 << interruption);
}

/* Return true if interrupts are enabled and a specific interrupt is
 * requested. */
bool InterruptManager::is_interrupt_requested() const {
    return ime && (ie & ix);
}

/* Return the bit corresponding to the enabled interrupt.
 * Interrupts with lower bit value have higher priority.
 * Returns -1 if no interrupts are enabled. */
int InterruptManager::get_enabled_interrupt() const {
    for (int bit_pos = 0; bit_pos < 5; bit_pos++) {
        if (((ie & ix) >> bit_pos) & 1) {
            return bit_pos;
        }
    }
    return -1;
}