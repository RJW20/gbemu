#include <iostream>
#include <cstdint>
#include "interrupt_manager.hpp"

// Reset all attributes.
void InterruptManager::reset() {
    ime = false;
    ie = 0;
    ix = 0;
}

/* Request an interrupt of the given type.
 * Sets the corresponding bit in ix. */
void InterruptManager::request(InterruptType interruption) {
    if (interruption == InterruptType::None) {
        return;
    }
    ix |= (1 << interruption);
}

/* Acknowledge an interrupt of the given type.
 * Resets the corresponding bit in ix. */
void InterruptManager::acknowledge(InterruptType interruption) {
    if (interruption == InterruptType::None) {
        return;
    }
    ix &= ~(1 << interruption);
}

/* Return true if interrupts are enabled and a specific interrupt is
 * requested. */
bool InterruptManager::is_interrupt_requested() const {
    return ime && (ie & ix);
}

/* Return the bit corresponding to the enabled interrupt.
 * Interrupts with lower bit value have higher priority.
 * Returns -1 if no interrupts are enabled. */
InterruptType InterruptManager::get_enabled() const {
    for (int bit_pos = 0; bit_pos < 5; bit_pos++) {
        if (((ie & ix) >> bit_pos) & 1) {
            return InterruptType(bit_pos);
        }
    }
    return InterruptType::None;
}

/* Return the program counter address corresponding to the given interrupt.
 * Should never be called when interruption is InterruptType::None. */
uint8_t InterruptManager::get_handler_address(InterruptType interruption) {
    if (interruption == InterruptType::None) {
        std::cout << "Do not call InterruptManager.get_handler_address with" <<
            " argument: " << interruption << std::endl;
        exit(10);
    } 
    return handler_addresses[interruption];
}