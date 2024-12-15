#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <unordered_map>
#include "register.hpp"
#include "interrupt_manager.hpp"
#include "timer.hpp"
#include "mmu.hpp"
#include "interrupt_manager.hpp"
#include "opcode.hpp"

class Cpu {
public:
    Cpu(Mmu* mmu, InterruptManager* interrupt_manager) :
        mmu(mmu), interrupt_manager(interrupt_manager) {initialise_opcodes();};
    ~Cpu() {};

    void tick();

private:
    Register reg;
    Mmu* mmu;
    InterruptManager* interrupt_manager;

    // Opcode dictionaries
    std::unordered_map<uint8_t, Opcode> opcodes;
    std::unordered_map<uint8_t, Opcode> cb_opcodes;

    void initialise_opcodes();

    // Variables for the opcodes to manipulate

    // Basis of cpu functionality
    void add(uint8_t value);
    void add_with_carry(uint8_t value);
    void subtract(uint8_t value);
    void subtract_with_carry(uint8_t value);
    void and_(uint8_t value);
    void or_(uint8_t value);
    void xor_(uint8_t value);
    void compare(uint8_t value);
    uint8_t increment(uint8_t value);
    uint8_t decrement(uint8_t value);

    void add_hl(uint16_t value);
    uint16_t add_signed8(uint16_t value16, int8_t value8);

    uint8_t swap(uint8_t value);
    void decimal_adjust_accumulator();
    void complement_accumulator();
    void complement_carry_flag();
    void set_carry_flag();
    void disable_interrupts();
    void enable_interrupts();

    uint8_t rotate_left_circular(uint8_t value);
    uint8_t rotate_left(uint8_t value);
    uint8_t rotate_right_circular(uint8_t value);
    uint8_t rotate_right(uint8_t value);
    uint8_t shift_left_arithmetic(uint8_t value);
    uint8_t shift_right_arithmetic(uint8_t value);
    uint8_t shift_right_logical(uint8_t value);
};

#endif