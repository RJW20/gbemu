#include <cstdint>
#include "cpu.hpp"

// ----------------------------------------------------------------------------
// Rotates and Shifts
// ----------------------------------------------------------------------------

/* Return the given 8-bit value rotated once (circularly) to the left.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::rotate_left_circular(uint8_t value) {
    return shift_left_arithmetic(value) | value >> 7;
}

/* Return the given 8-bit value shifted once to the left.
 * The 0th bit is set to the current value of the carry flag.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::rotate_left(uint8_t value) {
    bool carry = reg.flag_c;
    return shift_left_arithmetic(value) | (uint8_t) carry;
}

/* Return the given 8-bit value rotated once (circularly) to the right.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::rotate_right_circular(uint8_t value) {
    return shift_right_logical(value) | (uint8_t) (value << 7);
}

/* Return the given 8-bit value shifted once to the right.
 * The 7th bit is set to the current value of the carry flag.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::rotate_right(uint8_t value) {
    bool carry = reg.flag_c;
    return shift_right_logical(value) | (uint8_t) (carry << 7);
}

/* Return the given 8-bit value shifted once to the left.
 * The 0th bit is filled with 0.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::shift_left_arithmetic(uint8_t value) {
    uint8_t result = (uint8_t) (value << 1);
    reg.flag_z = (result == 0);
    reg.flag_n = false;
    reg.flag_h = false;
    reg.flag_c = (bool) (value >> 7);
    return result;
}

/* Return the given 8-bit value rotated once to the right.
 * The 7th bit is retained.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::shift_right_arithmetic(uint8_t value) {
    return shift_right_logical(value) | (value & 0x80);
}

/* Return the given 8-bit value rotated once to the right.
 * The 7th bit is filled with 0.
 * Sets the zero, subtract, half-carry and carry flags. */
uint8_t Cpu::shift_right_logical(uint8_t value) {
    uint8_t result = value >> 1;
    reg.flag_z = (result == 0);
    reg.flag_n = false;
    reg.flag_h = false;
    reg.flag_c = (bool) (value & 1);
    return result;
}