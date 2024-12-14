#include <cstdint>
#include "cpu.hpp"

// ----------------------------------------------------------------------------
// Rotates and Shifts
// ----------------------------------------------------------------------------

/* Return the given 8-bit value rotated once (circularly) to the left.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::rlc(uint8_t value) {
    return sla(value) | value >> 7;
}

/* Return the given 8-bit value shifted once to the left.
 * The 0th bit is set to the current value of the carry flag.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::rl(uint8_t value) {
    bool carry = reg.flag_c;
    return sla(value) | (uint8_t) carry;
}

/* Return the given 8-bit value rotated once (circularly) to the right.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::rrc(uint8_t value) {
    return srl(value) | (uint8_t) (value << 7);
}

/* Return the given 8-bit value shifted once to the right.
 * The 7th bit is set to the current value of the carry flag.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::rr(uint8_t value) {
    bool carry = reg.flag_c;
    return srl(value) | (uint8_t) (carry << 7);
}

/* Return the given 8-bit value shifted once to the left.
 * The 0th bit is set to 0.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::sla(uint8_t value) {
    uint8_t result = (uint8_t) (value << 1);
    reg.flag_z = (result == 0);
    reg.flag_n = false;
    reg.flag_h = false;
    reg.flag_c = (bool) (value >> 7);
    return result;
}

/* Return the given 8-bit value rotated once to the right.
 * The 7th bit is unchanged.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::sra(uint8_t value) {
    return srl(value) | (value & 0b10000000);
}

/* Return the given 8-bit value rotated once to the right.
 * The 7th bit is set to 0.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::srl(uint8_t value) {
    uint8_t result = value >> 1;
    reg.flag_z = (result == 0);
    reg.flag_n = false;
    reg.flag_h = false;
    reg.flag_c = (bool) (value & 1);
    return result;
}