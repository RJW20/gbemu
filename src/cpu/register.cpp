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

/* Return the flags register.
 * Has the following structure:
 * - Bit 7 = flag_z
 * - Bit 6 = flag_n
 * - Bit 5 = flag_h
 * - Bit 4 = flag_c
 * - Bits 0-3 = 0 */
uint8_t Register::f() {
    return (flag_z << 7) | (flag_n << 6) | (flag_h << 5) | (flag_c << 4);
}

/* Set the flags that make up the flags register.
 * Outputs to std::cerr if the trailing 4 bits in the given value are non-
 * zero. */
void Register::set_f(uint8_t value) {
    if (value & 0xF) {
        std::cerr << "The trailing 4 bits in register f must always be zero, "
            "ignoring them in value: " << std::hex << value << std::endl;
    }
    flag_z = value >> 7;
    flag_n = (value >> 6) & 1;
    flag_h = (value >> 5) & 1;
    flag_c = (value >> 4) & 1;
}

/* Return a 16 bit integer made up of register a as the first byte and register
 * f as the second byte. */
uint16_t Register::af() {
    return (uint16_t) (a << 8) | f();
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
 * of the second byte to register f. */
void Register::set_af(uint16_t value) {
    a = value >> 8;
    set_f((uint8_t) value);
}

/* Write the first byte of the given value to register b and the second byte to
 * register c. */
void Register::set_bc(uint16_t value) {
    b = value >> 8;
    c = value & 0xFF;
}

/* Write the first byte of the given value to register d and the second byte to
 * register e. */
void Register::set_de(uint16_t value) {
    d = value >> 8;
    e = value & 0xFF;
}

/* Write the first byte of the given value to register h and the second byte to
 * register l. */
void Register::set_hl(uint16_t value) {
    h = value >> 8;
    l = value & 0xFF;
}