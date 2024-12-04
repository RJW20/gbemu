#include <iostream>
#include <cstdint>
#include "register.hpp"

/* Return a 16 bit integer made up of register a as the first 8 bits and
 * register f as the second 8 bits. */
uint16_t Register::get_af() {
    return (uint16_t) (a << 8) | f;
}

/* Return a 16 bit integer made up of register b as the first 8 bits and
 * register c as the second 8 bits. */
uint16_t Register::get_bc() {
    return (uint16_t) (b << 8) | c;
}

/* Return a 16 bit integer made up of register d as the first 8 bits and
 * register e as the second 8 bits. */
uint16_t Register::get_de() {
    return (uint16_t) (d << 8) | e;
}

/* Return a 16 bit integer made up of register h as the first 8 bits and
 * register l as the second 8 bits. */
uint16_t Register::get_hl() {
    return (uint16_t) (h << 8) | l;
}

/* Set the first 8 bits of the given value to register a and the second 8 bits
 * to register f. */
void Register::set_af(uint16_t value) {
    a = (uint8_t) value >> 8;
    // TODO: prevent trailing 4 bits in F to be non-zero, and std::cerr if so
    f = (uint8_t) value & 0xFF;
}

/* Set the first 8 bits of the given value to register b and the second 8 bits
 * to register c. */
void Register::set_bc(uint16_t value) {
    b = (uint8_t) value >> 8;
    c = (uint8_t) value & 0xFF;
}

/* Set the first 8 bits of the given value to register d and the second 8 bits
 * to register e. */
void Register::set_de(uint16_t value) {
    d = (uint8_t) value >> 8;
    e = (uint8_t) value & 0xFF;
}

/* Set the first 8 bits of the given value to register h and the second 8 bits
 * to register l. */
void Register::set_hl(uint16_t value) {
    h = (uint8_t) value >> 8;
    l = (uint8_t) value & 0xFF;
}