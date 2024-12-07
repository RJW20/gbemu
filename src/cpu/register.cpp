#include <iostream>
#include <cstdint>
#include "register.hpp"

/* Return a 16 bit integer made up of register a as the first byte and register
 * f as the second byte. */
uint16_t Register::get_af() {
    return (uint16_t) (a << 8) | f;
}

/* Return a 16 bit integer made up of register b as the first byte and register
 * c as the second byte. */
uint16_t Register::get_bc() {
    return (uint16_t) (b << 8) | c;
}

/* Return a 16 bit integer made up of register d as the first byte and register
 * e as the second byte. */
uint16_t Register::get_de() {
    return (uint16_t) (d << 8) | e;
}

/* Return a 16 bit integer made up of register h as the first byte and register
 * l as the second byte. */
uint16_t Register::get_hl() {
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