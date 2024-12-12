#include <iostream>
#include <cstdint>
#include "register.hpp"

// Set all register values to their default GameBoy initial values
void Register::reset() {
    set_af(0x01B0);
    set_bc(0x0013);
    set_de(0x00D8);
    set_hl(0x014D);
    pc = 0x100;
    sp = 0xFFFE;
}

/* Return a 16 bit integer made up of register a as the first byte and register
 * f as the second byte. */
uint16_t Register::af() {
    return (uint16_t) (a << 8) | f;
}

/* Return a 16 bit integer made up of register b as the first byte and register
 * c as the second byte. */
uint16_t Register::bc() {
    return (uint16_t) (b << 8) | c;
}

/* Return a 16 bit integer made up of register d as the first byte and register
 * e as the second byte. */
uint16_t Register::de() {
    return (uint16_t) (d << 8) | e;
}

/* Return a 16 bit integer made up of register h as the first byte and register
 * l as the second byte. */
uint16_t Register::hl() {
    return (uint16_t) (h << 8) | l;
}

/* Write the first byte of the given value to register a and the first 4 bits
 * of the second byte to register f. 
 * Outputs to std::cerr if the trailing 4 bits in the second byte are non-
 * zero. */
void Register::set_af(uint16_t value) {
    a = (uint8_t) value >> 8;
    if (value & 0xF) {
        std::cerr << "The trailing 4 bits in register f must always be zero, "
            "ignoring them in value: " << std::hex << value << std::endl;
    }
    f = (uint8_t) value & 0xF0;
}

/* Write the first byte of the given value to register b and the second byte to
 * register c. */
void Register::set_bc(uint16_t value) {
    b = (uint8_t) value >> 8;
    c = (uint8_t) value & 0xFF;
}

/* Write the first byte of the given value to register d and the second byte to
 * register e. */
void Register::set_de(uint16_t value) {
    d = (uint8_t) value >> 8;
    e = (uint8_t) value & 0xFF;
}

/* Write the first byte of the given value to register h and the second byte to
 * register l. */
void Register::set_hl(uint16_t value) {
    h = (uint8_t) value >> 8;
    l = (uint8_t) value & 0xFF;
}

// Return the 7th bit of register f.
bool Register::flag_z() {
    return (bool) f >> 7;
}

// Return the 6th bit of register f.
bool Register::flag_n() {
    return (bool) (f >> 6) & 1;
}

// Return the 5th bit of register f.
bool Register::flag_h() {
    return (bool) (f >> 5) & 1;
}

// Return the 4th bit of register f.
bool Register::flag_c() {
    return (bool) (f >> 4) & 1;
}

/* Write the given value to the 7th bit of register f.
 * Should be set if the result of an operation is zero. */
void Register::set_flag_z(bool value) {
    f |= ((uint8_t) value) << 7;
}

/* Write the given value to the 6th bit of register f.
 * Should be set if the last operation was a subtraction. */
void Register::set_flag_n(bool value) {
    f |= ((uint8_t) value) << 6;
}

/* Write the given value to the 5th bit of register f.
 * Should be set if a carry occurred between the 3rd and 4th bits in the last
 * operation. */
void Register::set_flag_h(bool value) {
    f |= ((uint8_t) value) << 5;
}

/* Write the given value to the 4th bit of register f.
 * Should be set if the last operation resulted in a carry (an overflow). */
void Register::set_flag_c(bool value) {
    f |= ((uint8_t) value) << 4;
}