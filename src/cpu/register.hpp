#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <cstdint>

/* Simulates 8 8-bit registers that hold data that can be manipulated by the
 * cpu when carrying out instructions. */
class Register {
public:
    Register() {reset();};
    ~Register() {};

    uint8_t a;      // Accumulator
    uint8_t b, c;   // General-purpose registers
    uint8_t d, e;   // General-purpose registers
    uint8_t h, l;   // General-purpose registers
    uint16_t sp;    // Stack Pointer
    uint16_t pc;    // Program Counter

    // Flags register
    uint8_t f();
    void set_f(uint8_t);
    bool flag_z;  // Zero flag
    bool flag_n;  // Subtract flag
    bool flag_h;  // Half-carry flag
    bool flag_c;  // Carry flag

    void reset();

    // 16 bit getters
    uint16_t af();
    uint16_t bc();
    uint16_t de();
    uint16_t hl();

    // 16 bit setters
    void set_af(uint16_t value);
    void set_bc(uint16_t value);
    void set_de(uint16_t value);
    void set_hl(uint16_t value);
};

#endif