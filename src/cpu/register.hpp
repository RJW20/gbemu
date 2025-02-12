#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <cstdint>

/* Register
 * Simulates 8 8-bit registers that hold data that can be manipulated by the
 * CPU when carrying out instructions. */
class Register {
public:
    Register() { reset(); }

    void reset();

    uint8_t a;      // Accumulator
    uint8_t b, c;   // General-purpose registers
    uint8_t d, e;   // General-purpose registers
    uint8_t h, l;   // General-purpose registers
    uint16_t sp;    // Stack Pointer
    uint16_t pc;    // Program Counter

    // Flags register
    uint8_t f();
    void set_f(uint8_t value);
    bool flag_z : 1;  // Zero flag
    bool flag_n : 1;  // Subtract flag
    bool flag_h : 1;  // Half-carry flag
    bool flag_c : 1;  // Carry flag

    // 16 bit getters
    uint16_t af() { return (a << 8) | f(); }
    uint16_t bc() { return (b << 8) | c; }
    uint16_t de() { return (d << 8) | e; }
    uint16_t hl() { return (h << 8) | l; }

    // 16 bit setters
    void set_af(uint16_t value) { a = value >> 8; set_f(value & 0xFF); }
    void set_bc(uint16_t value) { b = value >> 8; c = value & 0xFF; }
    void set_de(uint16_t value) { d = value >> 8; e = value & 0xFF; }
    void set_hl(uint16_t value) { h = value >> 8; l = value & 0xFF; }
};

#endif