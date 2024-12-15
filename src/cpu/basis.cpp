#include <cstdint>
#include "cpu.hpp"

// ----------------------------------------------------------------------------
// 8-bit arithmetic
// ----------------------------------------------------------------------------

/* Set a to a + value.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
void Cpu::add(uint8_t value) {
    int result = reg.a + value;
    reg.flag_z = ((result & 0xFF) == 0);
    reg.flag_n = false;
    reg.flag_h = ((reg.a & 0xF) + (value & 0xF) > 0xF); // Bits 3 and 4
    reg.flag_c = (result > 0xFF);
    reg.a = (uint8_t) result;
}

/* Set a to a + value + the current value of the carry flag.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
void Cpu::add_with_carry(uint8_t value) {
    if (value != 0xFF || !reg.flag_c) {
        add(value + (uint8_t) reg.flag_c);
    }
    else {      // value + reg.flagc = 0x100
        reg.flag_z = (reg.a == 0);
        reg.flag_n = false;
        reg.flag_h = true;
        reg.flag_c = true;
    }
}

/* Set a to a - value.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
void Cpu::subtract(uint8_t value) {
    int result = reg.a - value;
    reg.flag_z = (result == 0);
    reg.flag_n = true;
    reg.flag_h = (reg.a & 0xF < value & 0xF); // Bits 3 and 4
    reg.flag_c = (result < 0);
    reg.a = (uint8_t) result;
}

/* Set a to a - value - the current value of the carry flag.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
void Cpu::subtract_with_carry(uint8_t value) {
    if (value != 0xFF || !reg.flag_c) {
        subtract(value + (uint8_t) reg.flag_c);
    }
    else {      // value + reg.flagc = 0x100
        reg.flag_z = (reg.a == 0);
        reg.flag_n = true;
        reg.flag_h = true;
        reg.flag_c = true;
    }
}

/* Set a to a & value.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
void Cpu::and_(uint8_t value) {
    reg.a &= value;
    reg.flag_z = (reg.a == 0);
    reg.flag_n = false;
    reg.flag_h = true;
    reg.flag_c = false;
}

/* Set a to a | value.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
void Cpu::or_(uint8_t value) {
    reg.a |= value;
    reg.flag_z = (reg.a == 0);
    reg.flag_n = false;
    reg.flag_h = false;
    reg.flag_c = false;
}

/* Set a to a ^ value.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
void Cpu::xor_(uint8_t value) {
    reg.a ^= value;
    reg.flag_z = (reg.a == 0);
    reg.flag_n = false;
    reg.flag_h = false;
    reg.flag_c = false;
}

/* Compare the given value with register a (by subtracting them). 
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
void Cpu::compare(uint8_t value) {
    int result = reg.a - value;
    reg.flag_z = (result == 0);
    reg.flag_n = true;
    reg.flag_h = (reg.a & 0xF < value & 0xF); // Bits 3 and 4
    reg.flag_c = (result < 0);
}

/* Return the given value incremented by 1.
 * Sets the zero, subtract and half-carry flags as necessary. */
uint8_t Cpu::increment(uint8_t value) {
    uint8_t result = value + 1;
    reg.flag_z = (result == 0);
    reg.flag_n = false;
    reg.flag_h = ((value & 0xF) == 0xF);    // Bits 3 and 4
    return result;
}

/* Return the given value decremented by 1.
 * Sets the zero, subtract and half-carry flags as necessary. */
uint8_t Cpu::decrement(uint8_t value) {
    uint8_t result = value - 1;
    reg.flag_z = (result == 0);
    reg.flag_n = true;
    reg.flag_h = ((value & 0xF) == 0);      // Bits 3 and 4
    return result;
}

// ----------------------------------------------------------------------------
// Rotates and Shifts
// ----------------------------------------------------------------------------

/* Return the given 8-bit value rotated once (circularly) to the left.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
uint8_t Cpu::rotate_left_circular(uint8_t value) {
    return shift_left_arithmetic(value) | value >> 7;
}

/* Return the given 8-bit value shifted once to the left.
 * The 0th bit is set to the current value of the carry flag.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
uint8_t Cpu::rotate_left(uint8_t value) {
    bool carry = reg.flag_c;
    return shift_left_arithmetic(value) | (uint8_t) carry;
}

/* Return the given 8-bit value rotated once (circularly) to the right.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
uint8_t Cpu::rotate_right_circular(uint8_t value) {
    return shift_right_logical(value) | (uint8_t) (value << 7);
}

/* Return the given 8-bit value shifted once to the right.
 * The 7th bit is set to the current value of the carry flag.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
uint8_t Cpu::rotate_right(uint8_t value) {
    bool carry = reg.flag_c;
    return shift_right_logical(value) | (uint8_t) (carry << 7);
}

/* Return the given 8-bit value shifted once to the left.
 * The 0th bit is filled with 0.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
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
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
uint8_t Cpu::shift_right_arithmetic(uint8_t value) {
    return shift_right_logical(value) | (value & 0x80);
}

/* Return the given 8-bit value rotated once to the right.
 * The 7th bit is filled with 0.
 * Sets the zero, subtract, half-carry and carry flags as necessary. */
uint8_t Cpu::shift_right_logical(uint8_t value) {
    uint8_t result = value >> 1;
    reg.flag_z = (result == 0);
    reg.flag_n = false;
    reg.flag_h = false;
    reg.flag_c = (bool) (value & 1);
    return result;
}