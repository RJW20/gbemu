#include <cstdint>
#include <utility>
#include <string>
#include <sstream>
#include <bitset>
#include "logger.hpp"
#include "interrupt_manager.hpp"

// Set the registers to their post boot ROM values.
void InterruptManager::reset() {
    ime = false;
    ie_ = 0;
    if_ = 0xE1;
}

/* Request an interrupt of the given type.
 * Sets the corresponding bit in if_. */
void InterruptManager::request(InterruptType interruption) {
    if (interruption == InterruptType::NONE) {
        return;
    }
    if_ |= (1 << std::to_underlying(interruption));
}

/* Acknowledge an interrupt of the given type.
 * Resets the corresponding bit in if_. */
void InterruptManager::acknowledge(InterruptType interruption) {
    if (interruption == InterruptType::NONE) {
        return;
    }
    if_ &= ~(1 << std::to_underlying(interruption));
}

/* Return the bit corresponding to the enabled interrupt.
 * Interrupts with lower bit value have higher priority.
 * Returns -1 if no interrupts are enabled. */
InterruptType InterruptManager::get_enabled() const {
    for (int bit_pos = 0; bit_pos < 5; bit_pos++) {
        if (((ie_ & if_) >> bit_pos) & 1) {
            return InterruptType(bit_pos);
        }
    }
    return InterruptType::NONE;
}

/* Return the program counter address corresponding to the given interrupt.
 * Should never be called when interruption is InterruptType::None. */
uint8_t InterruptManager::get_handler_address(InterruptType interruption) {
    if (interruption == InterruptType::NONE) {
        Log::error(
            "Do not call InterruptManager.get_handler_address with argument: "
            "InterruptType::NONE"
        );
        exit(10);
    } 
    return handler_addresses.at(std::to_underlying(interruption));
}

// Return a string representation of the InterruptManager.
std::string InterruptManager::representation() const {
    std::ostringstream repr;
    repr << "Interrupt Manager:" << std::hex
        << " IME = " << static_cast<int>(ime)
        << " IE = " << std::bitset<5>(ie())
        << " IF = " << std::bitset<5>(_if());
    return repr.str();
}