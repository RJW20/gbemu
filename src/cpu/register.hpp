#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <cstdint>

class Register {
public:
    Register() {reset();};
    ~Register() {};

    void reset();

    // 16 bit getters
    uint16_t get_af();
    uint16_t get_bc();
    uint16_t get_de();
    uint16_t get_hl();

    // 16 bit setters
    void set_af(uint16_t value);
    void set_bc(uint16_t value);
    void set_de(uint16_t value);
    void set_hl(uint16_t value);

private:
    uint8_t a, f;   // Accumulator and Flags
    uint8_t b, c;   // General-purpose registers
    uint8_t d, e;   // General-purpose registers
    uint8_t h, l;   // General-purpose registers
    uint16_t sp;    // Stack Pointer
    uint16_t pc;    // Program Counter
};

#endif