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

    // Variables for the opcodes to manipulate

    void initialise_opcodes();

    // Basis of cpu functionality
};

#endif