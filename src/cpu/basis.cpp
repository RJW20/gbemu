#include <cstdint>
#include "cpu.hpp"

// ----------------------------------------------------------------------------
// Rotates and Shifts
// ----------------------------------------------------------------------------

/* Return the given 8-bit value rotated once (circularly) to the left.
 * Sets the zero and carry flags. */
uint8_t Cpu::rlc(uint8_t value) {
    uint8_t result = (uint8_t) ((value << 1) | (value >> 7));
    reg.flag_z = (result == 0);
    reg.flag_c = (bool) (value >> 7);
    return result;
}

/* Return the given 8-bit value rotated once to the left.
 * The 0th bit is set to the current value of the carry flag.
 * Sets the zero and carry flags. */
uint8_t Cpu::rl(uint8_t value) {
    uint8_t result = (uint8_t) ((value << 1) | reg.flag_c);
    reg.flag_z = (result == 0);
    reg.flag_c = (bool) (value >> 7);
    return result;
}

/* Return the given 8-bit value rotated once (circularly) to the right.
 * Sets the zero and carry flags. */
uint8_t Cpu::rrc(uint8_t value) {
    uint8_t result = (uint8_t) ((value >> 1) | (value << 7));
    reg.flag_z = (result == 0);
    reg.flag_c = (bool) (value & 1);
    return result;
}

/* Return the given 8-bit value rotated once to the right.
 * The 7th bit is set to the current value of the carry flag.
 * Sets the zero and carry flags. */
uint8_t Cpu::rr(uint8_t value) {
    uint8_t result = (uint8_t) ((value >> 1) | reg.flag_c << 7);
    reg.flag_z = (result == 0);
    reg.flag_c = (bool) (value & 1);
    return result;
}