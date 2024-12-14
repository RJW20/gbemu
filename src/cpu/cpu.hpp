#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <unordered_map>
#include "register.hpp"
#include "interrupt_manager.hpp"
#include "timer.hpp"
#include "mmu.hpp"
#include "opcode.hpp"

class Cpu {
public:
    Cpu(Mmu* mmu) : mmu(mmu) {initialise_opcodes();};
    ~Cpu() {};

    void tick();

private:
    Register reg;
    Mmu* mmu;

    // Opcode dictionaries
    std::unordered_map<uint8_t, Opcode> opcodes;
    std::unordered_map<uint8_t, Opcode> cb_opcodes;

    void initialise_opcodes();

    // Variables for the opcodes to manipulate

    // Basis of cpu functionality
    void add(uint8_t value);
    void sub(uint8_t value);
    uint8_t increment(uint8_t value);
    uint8_t decrement(uint8_t value);

    uint8_t rotate_left_circular(uint8_t value);
    uint8_t rotate_left(uint8_t value);
    uint8_t rotate_right_circular(uint8_t value);
    uint8_t rotate_right(uint8_t value);
    uint8_t shift_left_arithmetic(uint8_t value);
    uint8_t shift_right_arithmetic(uint8_t value);
    uint8_t shift_right_logical(uint8_t value);
};

#endif