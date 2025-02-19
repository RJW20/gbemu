// This file is auto-generated.
// Do not modify this file directly.

#include <vector>
#include "cpu.hpp"
#include "opcode.hpp"

// NOP
Opcode Cpu::opcode_0x00() {
    std::vector<Step> steps;
    steps.push_back([this] { ; });
    return Opcode{0x00, "NOP", 1, 4, steps};
}

// LD BC, d16
Opcode Cpu::opcode_0x01() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8; });
    steps.push_back([this] { reg.set_bc(z16); });
    return Opcode{0x01, "LD BC, d16", 3, 12, steps};
}

// LD (BC), A
Opcode Cpu::opcode_0x02() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write(reg.bc(), reg.a); });
    return Opcode{0x02, "LD (BC), A", 1, 8, steps};
}

// INC BC
Opcode Cpu::opcode_0x03() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.set_bc(reg.bc() + 1); });
    return Opcode{0x03, "INC BC", 1, 8, steps};
}

// INC B
Opcode Cpu::opcode_0x04() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = increment(reg.b); });
    return Opcode{0x04, "INC B", 1, 4, steps};
}

// DEC B
Opcode Cpu::opcode_0x05() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = decrement(reg.b); });
    return Opcode{0x05, "DEC B", 1, 4, steps};
}

// LD B, d8
Opcode Cpu::opcode_0x06() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { reg.b = z8; });
    return Opcode{0x06, "LD B, d8", 2, 8, steps};
}

// RLCA
Opcode Cpu::opcode_0x07() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = rotate_left_circular(reg.a);
        reg.flag_z = false; });
    return Opcode{0x07, "RLCA", 1, 4, steps};
}

// LD (a16), SP
Opcode Cpu::opcode_0x08() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8; });
    steps.push_back([this] { mmu->write(z16, reg.sp & 0xFF); });
    steps.push_back([this] { mmu->write((uint16_t) (z16 + 1), reg.sp >> 8); });
    return Opcode{0x08, "LD (a16), SP", 3, 20, steps};
}

// ADD HL, BC
Opcode Cpu::opcode_0x09() {
    std::vector<Step> steps;
    steps.push_back([this] { add_hl(reg.bc()); });
    return Opcode{0x09, "ADD HL, BC", 1, 8, steps};
}

// LD A, (BC)
Opcode Cpu::opcode_0x0a() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.bc()); });
    steps.push_back([this] { reg.a = z8; });
    return Opcode{0x0a, "LD A, (BC)", 1, 8, steps};
}

// DEC BC
Opcode Cpu::opcode_0x0b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.set_bc(reg.bc() - 1); });
    return Opcode{0x0b, "DEC BC", 1, 8, steps};
}

// INC C
Opcode Cpu::opcode_0x0c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = increment(reg.c); });
    return Opcode{0x0c, "INC C", 1, 4, steps};
}

// DEC C
Opcode Cpu::opcode_0x0d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = decrement(reg.c); });
    return Opcode{0x0d, "DEC C", 1, 4, steps};
}

// LD C, d8
Opcode Cpu::opcode_0x0e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { reg.c = z8; });
    return Opcode{0x0e, "LD C, d8", 2, 8, steps};
}

// RRCA
Opcode Cpu::opcode_0x0f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = rotate_right_circular(reg.a);
        reg.flag_z = false; });
    return Opcode{0x0f, "RRCA", 1, 4, steps};
}

// STOP 0
Opcode Cpu::opcode_0x10() {
    std::vector<Step> steps;
    steps.push_back([this] { ; });
    return Opcode{0x10, "STOP 0", 1, 4, steps};
}

// LD DE, d16
Opcode Cpu::opcode_0x11() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8; });
    steps.push_back([this] { reg.set_de(z16); });
    return Opcode{0x11, "LD DE, d16", 3, 12, steps};
}

// LD (DE), A
Opcode Cpu::opcode_0x12() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write(reg.de(), reg.a); });
    return Opcode{0x12, "LD (DE), A", 1, 8, steps};
}

// INC DE
Opcode Cpu::opcode_0x13() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.set_de(reg.de() + 1); });
    return Opcode{0x13, "INC DE", 1, 8, steps};
}

// INC D
Opcode Cpu::opcode_0x14() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = increment(reg.d); });
    return Opcode{0x14, "INC D", 1, 4, steps};
}

// DEC D
Opcode Cpu::opcode_0x15() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = decrement(reg.d); });
    return Opcode{0x15, "DEC D", 1, 4, steps};
}

// LD D, d8
Opcode Cpu::opcode_0x16() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { reg.d = z8; });
    return Opcode{0x16, "LD D, d8", 2, 8, steps};
}

// RLA
Opcode Cpu::opcode_0x17() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = rotate_left(reg.a);
        reg.flag_z = false; });
    return Opcode{0x17, "RLA", 1, 4, steps};
}

// JR r8
Opcode Cpu::opcode_0x18() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { reg.pc = (uint16_t) (reg.pc + (int8_t) z8); });
    return Opcode{0x18, "JR r8", 2, 12, steps};
}

// ADD HL, DE
Opcode Cpu::opcode_0x19() {
    std::vector<Step> steps;
    steps.push_back([this] { add_hl(reg.de()); });
    return Opcode{0x19, "ADD HL, DE", 1, 8, steps};
}

// LD A, (DE)
Opcode Cpu::opcode_0x1a() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.de()); });
    steps.push_back([this] { reg.a = z8; });
    return Opcode{0x1a, "LD A, (DE)", 1, 8, steps};
}

// DEC DE
Opcode Cpu::opcode_0x1b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.set_de(reg.de() - 1); });
    return Opcode{0x1b, "DEC DE", 1, 8, steps};
}

// INC E
Opcode Cpu::opcode_0x1c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = increment(reg.e); });
    return Opcode{0x1c, "INC E", 1, 4, steps};
}

// DEC E
Opcode Cpu::opcode_0x1d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = decrement(reg.e); });
    return Opcode{0x1d, "DEC E", 1, 4, steps};
}

// LD E, d8
Opcode Cpu::opcode_0x1e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { reg.e = z8; });
    return Opcode{0x1e, "LD E, d8", 2, 8, steps};
}

// RRA
Opcode Cpu::opcode_0x1f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = rotate_right(reg.a);
        reg.flag_z = false; });
    return Opcode{0x1f, "RRA", 1, 4, steps};
}

// JR NZ, r8
Opcode Cpu::opcode_0x20() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++);
        if (reg.flag_z) early_exit = true; });
    steps.push_back([this] { reg.pc = (uint16_t) (reg.pc + (int8_t) z8); });
    return Opcode{0x20, "JR NZ, r8", 2, 12, steps};
}

// LD HL, d16
Opcode Cpu::opcode_0x21() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8; });
    steps.push_back([this] { reg.set_hl(z16); });
    return Opcode{0x21, "LD HL, d16", 3, 12, steps};
}

// LD (HL+), A
Opcode Cpu::opcode_0x22() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write(reg.hl(), reg.a);
        reg.set_hl(reg.hl() + 1); });
    return Opcode{0x22, "LD (HL+), A", 1, 8, steps};
}

// INC HL
Opcode Cpu::opcode_0x23() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.set_hl(reg.hl() + 1); });
    return Opcode{0x23, "INC HL", 1, 8, steps};
}

// INC H
Opcode Cpu::opcode_0x24() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = increment(reg.h); });
    return Opcode{0x24, "INC H", 1, 4, steps};
}

// DEC H
Opcode Cpu::opcode_0x25() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = decrement(reg.h); });
    return Opcode{0x25, "DEC H", 1, 4, steps};
}

// LD H, d8
Opcode Cpu::opcode_0x26() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { reg.h = z8; });
    return Opcode{0x26, "LD H, d8", 2, 8, steps};
}

// DAA
Opcode Cpu::opcode_0x27() {
    std::vector<Step> steps;
    steps.push_back([this] { decimal_adjust_accumulator(); });
    return Opcode{0x27, "DAA", 1, 4, steps};
}

// JR Z, r8
Opcode Cpu::opcode_0x28() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++);
        if (!reg.flag_z) early_exit = true; });
    steps.push_back([this] { reg.pc = (uint16_t) (reg.pc + (int8_t) z8); });
    return Opcode{0x28, "JR Z, r8", 2, 12, steps};
}

// ADD HL, HL
Opcode Cpu::opcode_0x29() {
    std::vector<Step> steps;
    steps.push_back([this] { add_hl(reg.hl()); });
    return Opcode{0x29, "ADD HL, HL", 1, 8, steps};
}

// LD A, (HL+)
Opcode Cpu::opcode_0x2a() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl());
        reg.set_hl(reg.hl() + 1); });
    steps.push_back([this] { reg.a = z8; });
    return Opcode{0x2a, "LD A, (HL+)", 1, 8, steps};
}

// DEC HL
Opcode Cpu::opcode_0x2b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.set_hl(reg.hl() - 1); });
    return Opcode{0x2b, "DEC HL", 1, 8, steps};
}

// INC L
Opcode Cpu::opcode_0x2c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = increment(reg.l); });
    return Opcode{0x2c, "INC L", 1, 4, steps};
}

// DEC L
Opcode Cpu::opcode_0x2d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = decrement(reg.l); });
    return Opcode{0x2d, "DEC L", 1, 4, steps};
}

// LD L, d8
Opcode Cpu::opcode_0x2e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { reg.l = z8; });
    return Opcode{0x2e, "LD L, d8", 2, 8, steps};
}

// CPL
Opcode Cpu::opcode_0x2f() {
    std::vector<Step> steps;
    steps.push_back([this] { complement_accumulator(); });
    return Opcode{0x2f, "CPL", 1, 4, steps};
}

// JR NC, r8
Opcode Cpu::opcode_0x30() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++);
        if (reg.flag_c) early_exit = true; });
    steps.push_back([this] { reg.pc = (uint16_t) (reg.pc + (int8_t) z8); });
    return Opcode{0x30, "JR NC, r8", 2, 12, steps};
}

// LD SP, d16
Opcode Cpu::opcode_0x31() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8; });
    steps.push_back([this] { reg.sp = z16; });
    return Opcode{0x31, "LD SP, d16", 3, 12, steps};
}

// LD (HL-), A
Opcode Cpu::opcode_0x32() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write(reg.hl(), reg.a);
        reg.set_hl(reg.hl() - 1); });
    return Opcode{0x32, "LD (HL-), A", 1, 8, steps};
}

// INC SP
Opcode Cpu::opcode_0x33() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp++; });
    return Opcode{0x33, "INC SP", 1, 8, steps};
}

// INC (HL)
Opcode Cpu::opcode_0x34() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = increment(z8); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x34, "INC (HL)", 1, 12, steps};
}

// DEC (HL)
Opcode Cpu::opcode_0x35() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = decrement(z8); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x35, "DEC (HL)", 1, 12, steps};
}

// LD (HL), d8
Opcode Cpu::opcode_0x36() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x36, "LD (HL), d8", 2, 12, steps};
}

// SCF
Opcode Cpu::opcode_0x37() {
    std::vector<Step> steps;
    steps.push_back([this] { set_carry_flag(); });
    return Opcode{0x37, "SCF", 1, 4, steps};
}

// JR C, r8
Opcode Cpu::opcode_0x38() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++);
        if (!reg.flag_c) early_exit = true; });
    steps.push_back([this] { reg.pc = (uint16_t) (reg.pc + (int8_t) z8); });
    return Opcode{0x38, "JR C, r8", 2, 12, steps};
}

// ADD HL, SP
Opcode Cpu::opcode_0x39() {
    std::vector<Step> steps;
    steps.push_back([this] { add_hl(reg.sp); });
    return Opcode{0x39, "ADD HL, SP", 1, 8, steps};
}

// LD A, (HL-)
Opcode Cpu::opcode_0x3a() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl());
        reg.set_hl(reg.hl() - 1); });
    steps.push_back([this] { reg.a = z8; });
    return Opcode{0x3a, "LD A, (HL-)", 1, 8, steps};
}

// DEC SP
Opcode Cpu::opcode_0x3b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    return Opcode{0x3b, "DEC SP", 1, 8, steps};
}

// INC A
Opcode Cpu::opcode_0x3c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = increment(reg.a); });
    return Opcode{0x3c, "INC A", 1, 4, steps};
}

// DEC A
Opcode Cpu::opcode_0x3d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = decrement(reg.a); });
    return Opcode{0x3d, "DEC A", 1, 4, steps};
}

// LD A, d8
Opcode Cpu::opcode_0x3e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { reg.a = z8; });
    return Opcode{0x3e, "LD A, d8", 2, 8, steps};
}

// CCF
Opcode Cpu::opcode_0x3f() {
    std::vector<Step> steps;
    steps.push_back([this] { complement_carry_flag(); });
    return Opcode{0x3f, "CCF", 1, 4, steps};
}

// LD B, B
Opcode Cpu::opcode_0x40() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reg.b; });
    return Opcode{0x40, "LD B, B", 1, 4, steps};
}

// LD B, C
Opcode Cpu::opcode_0x41() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reg.c; });
    return Opcode{0x41, "LD B, C", 1, 4, steps};
}

// LD B, D
Opcode Cpu::opcode_0x42() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reg.d; });
    return Opcode{0x42, "LD B, D", 1, 4, steps};
}

// LD B, E
Opcode Cpu::opcode_0x43() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reg.e; });
    return Opcode{0x43, "LD B, E", 1, 4, steps};
}

// LD B, H
Opcode Cpu::opcode_0x44() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reg.h; });
    return Opcode{0x44, "LD B, H", 1, 4, steps};
}

// LD B, L
Opcode Cpu::opcode_0x45() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reg.l; });
    return Opcode{0x45, "LD B, L", 1, 4, steps};
}

// LD B, (HL)
Opcode Cpu::opcode_0x46() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { reg.b = z8; });
    return Opcode{0x46, "LD B, (HL)", 1, 8, steps};
}

// LD B, A
Opcode Cpu::opcode_0x47() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reg.a; });
    return Opcode{0x47, "LD B, A", 1, 4, steps};
}

// LD C, B
Opcode Cpu::opcode_0x48() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reg.b; });
    return Opcode{0x48, "LD C, B", 1, 4, steps};
}

// LD C, C
Opcode Cpu::opcode_0x49() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reg.c; });
    return Opcode{0x49, "LD C, C", 1, 4, steps};
}

// LD C, D
Opcode Cpu::opcode_0x4a() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reg.d; });
    return Opcode{0x4a, "LD C, D", 1, 4, steps};
}

// LD C, E
Opcode Cpu::opcode_0x4b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reg.e; });
    return Opcode{0x4b, "LD C, E", 1, 4, steps};
}

// LD C, H
Opcode Cpu::opcode_0x4c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reg.h; });
    return Opcode{0x4c, "LD C, H", 1, 4, steps};
}

// LD C, L
Opcode Cpu::opcode_0x4d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reg.l; });
    return Opcode{0x4d, "LD C, L", 1, 4, steps};
}

// LD C, (HL)
Opcode Cpu::opcode_0x4e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { reg.c = z8; });
    return Opcode{0x4e, "LD C, (HL)", 1, 8, steps};
}

// LD C, A
Opcode Cpu::opcode_0x4f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reg.a; });
    return Opcode{0x4f, "LD C, A", 1, 4, steps};
}

// LD D, B
Opcode Cpu::opcode_0x50() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reg.b; });
    return Opcode{0x50, "LD D, B", 1, 4, steps};
}

// LD D, C
Opcode Cpu::opcode_0x51() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reg.c; });
    return Opcode{0x51, "LD D, C", 1, 4, steps};
}

// LD D, D
Opcode Cpu::opcode_0x52() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reg.d; });
    return Opcode{0x52, "LD D, D", 1, 4, steps};
}

// LD D, E
Opcode Cpu::opcode_0x53() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reg.e; });
    return Opcode{0x53, "LD D, E", 1, 4, steps};
}

// LD D, H
Opcode Cpu::opcode_0x54() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reg.h; });
    return Opcode{0x54, "LD D, H", 1, 4, steps};
}

// LD D, L
Opcode Cpu::opcode_0x55() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reg.l; });
    return Opcode{0x55, "LD D, L", 1, 4, steps};
}

// LD D, (HL)
Opcode Cpu::opcode_0x56() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { reg.d = z8; });
    return Opcode{0x56, "LD D, (HL)", 1, 8, steps};
}

// LD D, A
Opcode Cpu::opcode_0x57() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reg.a; });
    return Opcode{0x57, "LD D, A", 1, 4, steps};
}

// LD E, B
Opcode Cpu::opcode_0x58() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reg.b; });
    return Opcode{0x58, "LD E, B", 1, 4, steps};
}

// LD E, C
Opcode Cpu::opcode_0x59() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reg.c; });
    return Opcode{0x59, "LD E, C", 1, 4, steps};
}

// LD E, D
Opcode Cpu::opcode_0x5a() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reg.d; });
    return Opcode{0x5a, "LD E, D", 1, 4, steps};
}

// LD E, E
Opcode Cpu::opcode_0x5b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reg.e; });
    return Opcode{0x5b, "LD E, E", 1, 4, steps};
}

// LD E, H
Opcode Cpu::opcode_0x5c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reg.h; });
    return Opcode{0x5c, "LD E, H", 1, 4, steps};
}

// LD E, L
Opcode Cpu::opcode_0x5d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reg.l; });
    return Opcode{0x5d, "LD E, L", 1, 4, steps};
}

// LD E, (HL)
Opcode Cpu::opcode_0x5e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { reg.e = z8; });
    return Opcode{0x5e, "LD E, (HL)", 1, 8, steps};
}

// LD E, A
Opcode Cpu::opcode_0x5f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reg.a; });
    return Opcode{0x5f, "LD E, A", 1, 4, steps};
}

// LD H, B
Opcode Cpu::opcode_0x60() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reg.b; });
    return Opcode{0x60, "LD H, B", 1, 4, steps};
}

// LD H, C
Opcode Cpu::opcode_0x61() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reg.c; });
    return Opcode{0x61, "LD H, C", 1, 4, steps};
}

// LD H, D
Opcode Cpu::opcode_0x62() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reg.d; });
    return Opcode{0x62, "LD H, D", 1, 4, steps};
}

// LD H, E
Opcode Cpu::opcode_0x63() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reg.e; });
    return Opcode{0x63, "LD H, E", 1, 4, steps};
}

// LD H, H
Opcode Cpu::opcode_0x64() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reg.h; });
    return Opcode{0x64, "LD H, H", 1, 4, steps};
}

// LD H, L
Opcode Cpu::opcode_0x65() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reg.l; });
    return Opcode{0x65, "LD H, L", 1, 4, steps};
}

// LD H, (HL)
Opcode Cpu::opcode_0x66() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { reg.h = z8; });
    return Opcode{0x66, "LD H, (HL)", 1, 8, steps};
}

// LD H, A
Opcode Cpu::opcode_0x67() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reg.a; });
    return Opcode{0x67, "LD H, A", 1, 4, steps};
}

// LD L, B
Opcode Cpu::opcode_0x68() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reg.b; });
    return Opcode{0x68, "LD L, B", 1, 4, steps};
}

// LD L, C
Opcode Cpu::opcode_0x69() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reg.c; });
    return Opcode{0x69, "LD L, C", 1, 4, steps};
}

// LD L, D
Opcode Cpu::opcode_0x6a() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reg.d; });
    return Opcode{0x6a, "LD L, D", 1, 4, steps};
}

// LD L, E
Opcode Cpu::opcode_0x6b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reg.e; });
    return Opcode{0x6b, "LD L, E", 1, 4, steps};
}

// LD L, H
Opcode Cpu::opcode_0x6c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reg.h; });
    return Opcode{0x6c, "LD L, H", 1, 4, steps};
}

// LD L, L
Opcode Cpu::opcode_0x6d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reg.l; });
    return Opcode{0x6d, "LD L, L", 1, 4, steps};
}

// LD L, (HL)
Opcode Cpu::opcode_0x6e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { reg.l = z8; });
    return Opcode{0x6e, "LD L, (HL)", 1, 8, steps};
}

// LD L, A
Opcode Cpu::opcode_0x6f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reg.a; });
    return Opcode{0x6f, "LD L, A", 1, 4, steps};
}

// LD (HL), B
Opcode Cpu::opcode_0x70() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write(reg.hl(), reg.b); });
    return Opcode{0x70, "LD (HL), B", 1, 8, steps};
}

// LD (HL), C
Opcode Cpu::opcode_0x71() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write(reg.hl(), reg.c); });
    return Opcode{0x71, "LD (HL), C", 1, 8, steps};
}

// LD (HL), D
Opcode Cpu::opcode_0x72() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write(reg.hl(), reg.d); });
    return Opcode{0x72, "LD (HL), D", 1, 8, steps};
}

// LD (HL), E
Opcode Cpu::opcode_0x73() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write(reg.hl(), reg.e); });
    return Opcode{0x73, "LD (HL), E", 1, 8, steps};
}

// LD (HL), H
Opcode Cpu::opcode_0x74() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write(reg.hl(), reg.h); });
    return Opcode{0x74, "LD (HL), H", 1, 8, steps};
}

// LD (HL), L
Opcode Cpu::opcode_0x75() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write(reg.hl(), reg.l); });
    return Opcode{0x75, "LD (HL), L", 1, 8, steps};
}

// HALT
Opcode Cpu::opcode_0x76() {
    std::vector<Step> steps;
    steps.push_back([this] { ; });
    return Opcode{0x76, "HALT", 1, 4, steps};
}

// LD (HL), A
Opcode Cpu::opcode_0x77() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write(reg.hl(), reg.a); });
    return Opcode{0x77, "LD (HL), A", 1, 8, steps};
}

// LD A, B
Opcode Cpu::opcode_0x78() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reg.b; });
    return Opcode{0x78, "LD A, B", 1, 4, steps};
}

// LD A, C
Opcode Cpu::opcode_0x79() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reg.c; });
    return Opcode{0x79, "LD A, C", 1, 4, steps};
}

// LD A, D
Opcode Cpu::opcode_0x7a() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reg.d; });
    return Opcode{0x7a, "LD A, D", 1, 4, steps};
}

// LD A, E
Opcode Cpu::opcode_0x7b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reg.e; });
    return Opcode{0x7b, "LD A, E", 1, 4, steps};
}

// LD A, H
Opcode Cpu::opcode_0x7c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reg.h; });
    return Opcode{0x7c, "LD A, H", 1, 4, steps};
}

// LD A, L
Opcode Cpu::opcode_0x7d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reg.l; });
    return Opcode{0x7d, "LD A, L", 1, 4, steps};
}

// LD A, (HL)
Opcode Cpu::opcode_0x7e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { reg.a = z8; });
    return Opcode{0x7e, "LD A, (HL)", 1, 8, steps};
}

// LD A, A
Opcode Cpu::opcode_0x7f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reg.a; });
    return Opcode{0x7f, "LD A, A", 1, 4, steps};
}

// ADD A, B
Opcode Cpu::opcode_0x80() {
    std::vector<Step> steps;
    steps.push_back([this] { add(reg.b); });
    return Opcode{0x80, "ADD A, B", 1, 4, steps};
}

// ADD A, C
Opcode Cpu::opcode_0x81() {
    std::vector<Step> steps;
    steps.push_back([this] { add(reg.c); });
    return Opcode{0x81, "ADD A, C", 1, 4, steps};
}

// ADD A, D
Opcode Cpu::opcode_0x82() {
    std::vector<Step> steps;
    steps.push_back([this] { add(reg.d); });
    return Opcode{0x82, "ADD A, D", 1, 4, steps};
}

// ADD A, E
Opcode Cpu::opcode_0x83() {
    std::vector<Step> steps;
    steps.push_back([this] { add(reg.e); });
    return Opcode{0x83, "ADD A, E", 1, 4, steps};
}

// ADD A, H
Opcode Cpu::opcode_0x84() {
    std::vector<Step> steps;
    steps.push_back([this] { add(reg.h); });
    return Opcode{0x84, "ADD A, H", 1, 4, steps};
}

// ADD A, L
Opcode Cpu::opcode_0x85() {
    std::vector<Step> steps;
    steps.push_back([this] { add(reg.l); });
    return Opcode{0x85, "ADD A, L", 1, 4, steps};
}

// ADD A, (HL)
Opcode Cpu::opcode_0x86() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { add(z8); });
    return Opcode{0x86, "ADD A, (HL)", 1, 8, steps};
}

// ADD A, A
Opcode Cpu::opcode_0x87() {
    std::vector<Step> steps;
    steps.push_back([this] { add(reg.a); });
    return Opcode{0x87, "ADD A, A", 1, 4, steps};
}

// ADC A, B
Opcode Cpu::opcode_0x88() {
    std::vector<Step> steps;
    steps.push_back([this] { add_with_carry(reg.b); });
    return Opcode{0x88, "ADC A, B", 1, 4, steps};
}

// ADC A, C
Opcode Cpu::opcode_0x89() {
    std::vector<Step> steps;
    steps.push_back([this] { add_with_carry(reg.c); });
    return Opcode{0x89, "ADC A, C", 1, 4, steps};
}

// ADC A, D
Opcode Cpu::opcode_0x8a() {
    std::vector<Step> steps;
    steps.push_back([this] { add_with_carry(reg.d); });
    return Opcode{0x8a, "ADC A, D", 1, 4, steps};
}

// ADC A, E
Opcode Cpu::opcode_0x8b() {
    std::vector<Step> steps;
    steps.push_back([this] { add_with_carry(reg.e); });
    return Opcode{0x8b, "ADC A, E", 1, 4, steps};
}

// ADC A, H
Opcode Cpu::opcode_0x8c() {
    std::vector<Step> steps;
    steps.push_back([this] { add_with_carry(reg.h); });
    return Opcode{0x8c, "ADC A, H", 1, 4, steps};
}

// ADC A, L
Opcode Cpu::opcode_0x8d() {
    std::vector<Step> steps;
    steps.push_back([this] { add_with_carry(reg.l); });
    return Opcode{0x8d, "ADC A, L", 1, 4, steps};
}

// ADC A, (HL)
Opcode Cpu::opcode_0x8e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { add_with_carry(z8); });
    return Opcode{0x8e, "ADC A, (HL)", 1, 8, steps};
}

// ADC A, A
Opcode Cpu::opcode_0x8f() {
    std::vector<Step> steps;
    steps.push_back([this] { add_with_carry(reg.a); });
    return Opcode{0x8f, "ADC A, A", 1, 4, steps};
}

// SUB B
Opcode Cpu::opcode_0x90() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract(reg.b); });
    return Opcode{0x90, "SUB B", 1, 4, steps};
}

// SUB C
Opcode Cpu::opcode_0x91() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract(reg.c); });
    return Opcode{0x91, "SUB C", 1, 4, steps};
}

// SUB D
Opcode Cpu::opcode_0x92() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract(reg.d); });
    return Opcode{0x92, "SUB D", 1, 4, steps};
}

// SUB E
Opcode Cpu::opcode_0x93() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract(reg.e); });
    return Opcode{0x93, "SUB E", 1, 4, steps};
}

// SUB H
Opcode Cpu::opcode_0x94() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract(reg.h); });
    return Opcode{0x94, "SUB H", 1, 4, steps};
}

// SUB L
Opcode Cpu::opcode_0x95() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract(reg.l); });
    return Opcode{0x95, "SUB L", 1, 4, steps};
}

// SUB (HL)
Opcode Cpu::opcode_0x96() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { subtract(z8); });
    return Opcode{0x96, "SUB (HL)", 1, 8, steps};
}

// SUB A
Opcode Cpu::opcode_0x97() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract(reg.a); });
    return Opcode{0x97, "SUB A", 1, 4, steps};
}

// SBC A, B
Opcode Cpu::opcode_0x98() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract_with_carry(reg.b); });
    return Opcode{0x98, "SBC A, B", 1, 4, steps};
}

// SBC A, C
Opcode Cpu::opcode_0x99() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract_with_carry(reg.c); });
    return Opcode{0x99, "SBC A, C", 1, 4, steps};
}

// SBC A, D
Opcode Cpu::opcode_0x9a() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract_with_carry(reg.d); });
    return Opcode{0x9a, "SBC A, D", 1, 4, steps};
}

// SBC A, E
Opcode Cpu::opcode_0x9b() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract_with_carry(reg.e); });
    return Opcode{0x9b, "SBC A, E", 1, 4, steps};
}

// SBC A, H
Opcode Cpu::opcode_0x9c() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract_with_carry(reg.h); });
    return Opcode{0x9c, "SBC A, H", 1, 4, steps};
}

// SBC A, L
Opcode Cpu::opcode_0x9d() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract_with_carry(reg.l); });
    return Opcode{0x9d, "SBC A, L", 1, 4, steps};
}

// SBC A, (HL)
Opcode Cpu::opcode_0x9e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { subtract_with_carry(z8); });
    return Opcode{0x9e, "SBC A, (HL)", 1, 8, steps};
}

// SBC A, A
Opcode Cpu::opcode_0x9f() {
    std::vector<Step> steps;
    steps.push_back([this] { subtract_with_carry(reg.a); });
    return Opcode{0x9f, "SBC A, A", 1, 4, steps};
}

// AND B
Opcode Cpu::opcode_0xa0() {
    std::vector<Step> steps;
    steps.push_back([this] { and_(reg.b); });
    return Opcode{0xa0, "AND B", 1, 4, steps};
}

// AND C
Opcode Cpu::opcode_0xa1() {
    std::vector<Step> steps;
    steps.push_back([this] { and_(reg.c); });
    return Opcode{0xa1, "AND C", 1, 4, steps};
}

// AND D
Opcode Cpu::opcode_0xa2() {
    std::vector<Step> steps;
    steps.push_back([this] { and_(reg.d); });
    return Opcode{0xa2, "AND D", 1, 4, steps};
}

// AND E
Opcode Cpu::opcode_0xa3() {
    std::vector<Step> steps;
    steps.push_back([this] { and_(reg.e); });
    return Opcode{0xa3, "AND E", 1, 4, steps};
}

// AND H
Opcode Cpu::opcode_0xa4() {
    std::vector<Step> steps;
    steps.push_back([this] { and_(reg.h); });
    return Opcode{0xa4, "AND H", 1, 4, steps};
}

// AND L
Opcode Cpu::opcode_0xa5() {
    std::vector<Step> steps;
    steps.push_back([this] { and_(reg.l); });
    return Opcode{0xa5, "AND L", 1, 4, steps};
}

// AND (HL)
Opcode Cpu::opcode_0xa6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { and_(z8); });
    return Opcode{0xa6, "AND (HL)", 1, 8, steps};
}

// AND A
Opcode Cpu::opcode_0xa7() {
    std::vector<Step> steps;
    steps.push_back([this] { and_(reg.a); });
    return Opcode{0xa7, "AND A", 1, 4, steps};
}

// XOR B
Opcode Cpu::opcode_0xa8() {
    std::vector<Step> steps;
    steps.push_back([this] { xor_(reg.b); });
    return Opcode{0xa8, "XOR B", 1, 4, steps};
}

// XOR C
Opcode Cpu::opcode_0xa9() {
    std::vector<Step> steps;
    steps.push_back([this] { xor_(reg.c); });
    return Opcode{0xa9, "XOR C", 1, 4, steps};
}

// XOR D
Opcode Cpu::opcode_0xaa() {
    std::vector<Step> steps;
    steps.push_back([this] { xor_(reg.d); });
    return Opcode{0xaa, "XOR D", 1, 4, steps};
}

// XOR E
Opcode Cpu::opcode_0xab() {
    std::vector<Step> steps;
    steps.push_back([this] { xor_(reg.e); });
    return Opcode{0xab, "XOR E", 1, 4, steps};
}

// XOR H
Opcode Cpu::opcode_0xac() {
    std::vector<Step> steps;
    steps.push_back([this] { xor_(reg.h); });
    return Opcode{0xac, "XOR H", 1, 4, steps};
}

// XOR L
Opcode Cpu::opcode_0xad() {
    std::vector<Step> steps;
    steps.push_back([this] { xor_(reg.l); });
    return Opcode{0xad, "XOR L", 1, 4, steps};
}

// XOR (HL)
Opcode Cpu::opcode_0xae() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { xor_(z8); });
    return Opcode{0xae, "XOR (HL)", 1, 8, steps};
}

// XOR A
Opcode Cpu::opcode_0xaf() {
    std::vector<Step> steps;
    steps.push_back([this] { xor_(reg.a); });
    return Opcode{0xaf, "XOR A", 1, 4, steps};
}

// OR B
Opcode Cpu::opcode_0xb0() {
    std::vector<Step> steps;
    steps.push_back([this] { or_(reg.b); });
    return Opcode{0xb0, "OR B", 1, 4, steps};
}

// OR C
Opcode Cpu::opcode_0xb1() {
    std::vector<Step> steps;
    steps.push_back([this] { or_(reg.c); });
    return Opcode{0xb1, "OR C", 1, 4, steps};
}

// OR D
Opcode Cpu::opcode_0xb2() {
    std::vector<Step> steps;
    steps.push_back([this] { or_(reg.d); });
    return Opcode{0xb2, "OR D", 1, 4, steps};
}

// OR E
Opcode Cpu::opcode_0xb3() {
    std::vector<Step> steps;
    steps.push_back([this] { or_(reg.e); });
    return Opcode{0xb3, "OR E", 1, 4, steps};
}

// OR H
Opcode Cpu::opcode_0xb4() {
    std::vector<Step> steps;
    steps.push_back([this] { or_(reg.h); });
    return Opcode{0xb4, "OR H", 1, 4, steps};
}

// OR L
Opcode Cpu::opcode_0xb5() {
    std::vector<Step> steps;
    steps.push_back([this] { or_(reg.l); });
    return Opcode{0xb5, "OR L", 1, 4, steps};
}

// OR (HL)
Opcode Cpu::opcode_0xb6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { or_(z8); });
    return Opcode{0xb6, "OR (HL)", 1, 8, steps};
}

// OR A
Opcode Cpu::opcode_0xb7() {
    std::vector<Step> steps;
    steps.push_back([this] { or_(reg.a); });
    return Opcode{0xb7, "OR A", 1, 4, steps};
}

// CP B
Opcode Cpu::opcode_0xb8() {
    std::vector<Step> steps;
    steps.push_back([this] { compare(reg.b); });
    return Opcode{0xb8, "CP B", 1, 4, steps};
}

// CP C
Opcode Cpu::opcode_0xb9() {
    std::vector<Step> steps;
    steps.push_back([this] { compare(reg.c); });
    return Opcode{0xb9, "CP C", 1, 4, steps};
}

// CP D
Opcode Cpu::opcode_0xba() {
    std::vector<Step> steps;
    steps.push_back([this] { compare(reg.d); });
    return Opcode{0xba, "CP D", 1, 4, steps};
}

// CP E
Opcode Cpu::opcode_0xbb() {
    std::vector<Step> steps;
    steps.push_back([this] { compare(reg.e); });
    return Opcode{0xbb, "CP E", 1, 4, steps};
}

// CP H
Opcode Cpu::opcode_0xbc() {
    std::vector<Step> steps;
    steps.push_back([this] { compare(reg.h); });
    return Opcode{0xbc, "CP H", 1, 4, steps};
}

// CP L
Opcode Cpu::opcode_0xbd() {
    std::vector<Step> steps;
    steps.push_back([this] { compare(reg.l); });
    return Opcode{0xbd, "CP L", 1, 4, steps};
}

// CP (HL)
Opcode Cpu::opcode_0xbe() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { compare(z8); });
    return Opcode{0xbe, "CP (HL)", 1, 8, steps};
}

// CP A
Opcode Cpu::opcode_0xbf() {
    std::vector<Step> steps;
    steps.push_back([this] { compare(reg.a); });
    return Opcode{0xbf, "CP A", 1, 4, steps};
}

// RET NZ
Opcode Cpu::opcode_0xc0() {
    std::vector<Step> steps;
    steps.push_back([this] { if (reg.flag_z) early_exit = true; });
    steps.push_back([this] { z8 = mmu->read(reg.sp++); });
    steps.push_back([this] { z16 = (mmu->read(reg.sp++) << 8) | z8; });
    steps.push_back([this] { reg.pc = z16; });
    return Opcode{0xc0, "RET NZ", 1, 20, steps};
}

// POP BC
Opcode Cpu::opcode_0xc1() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.sp++); });
    steps.push_back([this] { z16 = (mmu->read(reg.sp++) << 8) | z8; });
    steps.push_back([this] { reg.set_bc(z16); });
    return Opcode{0xc1, "POP BC", 1, 12, steps};
}

// JP NZ, a16
Opcode Cpu::opcode_0xc2() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8;
        if (reg.flag_z) early_exit = true; });
    steps.push_back([this] { reg.pc = z16; });
    return Opcode{0xc2, "JP NZ, a16", 3, 16, steps};
}

// JP a16
Opcode Cpu::opcode_0xc3() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8; });
    steps.push_back([this] { reg.pc = z16; });
    return Opcode{0xc3, "JP a16", 3, 16, steps};
}

// CALL NZ, a16
Opcode Cpu::opcode_0xc4() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8;
        if (reg.flag_z) early_exit = true; });
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = z16; });
    return Opcode{0xc4, "CALL NZ, a16", 3, 24, steps};
}

// PUSH BC
Opcode Cpu::opcode_0xc5() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.b); });
    steps.push_back([this] { mmu->write(reg.sp, reg.c); });
    return Opcode{0xc5, "PUSH BC", 1, 16, steps};
}

// ADD A, d8
Opcode Cpu::opcode_0xc6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { add(z8); });
    return Opcode{0xc6, "ADD A, d8", 2, 8, steps};
}

// RST 00H
Opcode Cpu::opcode_0xc7() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = 0x00; });
    return Opcode{0xc7, "RST 00H", 1, 16, steps};
}

// RET Z
Opcode Cpu::opcode_0xc8() {
    std::vector<Step> steps;
    steps.push_back([this] { if (!reg.flag_z) early_exit = true; });
    steps.push_back([this] { z8 = mmu->read(reg.sp++); });
    steps.push_back([this] { z16 = (mmu->read(reg.sp++) << 8) | z8; });
    steps.push_back([this] { reg.pc = z16; });
    return Opcode{0xc8, "RET Z", 1, 20, steps};
}

// RET
Opcode Cpu::opcode_0xc9() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.sp++); });
    steps.push_back([this] { z16 = (mmu->read(reg.sp++) << 8) | z8; });
    steps.push_back([this] { reg.pc = z16; });
    return Opcode{0xc9, "RET", 1, 16, steps};
}

// JP Z, a16
Opcode Cpu::opcode_0xca() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8;
        if (!reg.flag_z) early_exit = true; });
    steps.push_back([this] { reg.pc = z16; });
    return Opcode{0xca, "JP Z, a16", 3, 16, steps};
}

// PREFIX CB
Opcode Cpu::opcode_0xcb() {
    std::vector<Step> steps;
    steps.push_back([this] { ; });
    return Opcode{0xcb, "PREFIX CB", 1, 4, steps};
}

// CALL Z, a16
Opcode Cpu::opcode_0xcc() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8;
        if (!reg.flag_z) early_exit = true; });
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = z16; });
    return Opcode{0xcc, "CALL Z, a16", 3, 24, steps};
}

// CALL a16
Opcode Cpu::opcode_0xcd() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8; });
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = z16; });
    return Opcode{0xcd, "CALL a16", 3, 24, steps};
}

// ADC A, d8
Opcode Cpu::opcode_0xce() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { add_with_carry(z8); });
    return Opcode{0xce, "ADC A, d8", 2, 8, steps};
}

// RST 08H
Opcode Cpu::opcode_0xcf() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = 0x08; });
    return Opcode{0xcf, "RST 08H", 1, 16, steps};
}

// RET NC
Opcode Cpu::opcode_0xd0() {
    std::vector<Step> steps;
    steps.push_back([this] { if (reg.flag_c) early_exit = true; });
    steps.push_back([this] { z8 = mmu->read(reg.sp++); });
    steps.push_back([this] { z16 = (mmu->read(reg.sp++) << 8) | z8; });
    steps.push_back([this] { reg.pc = z16; });
    return Opcode{0xd0, "RET NC", 1, 20, steps};
}

// POP DE
Opcode Cpu::opcode_0xd1() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.sp++); });
    steps.push_back([this] { z16 = (mmu->read(reg.sp++) << 8) | z8; });
    steps.push_back([this] { reg.set_de(z16); });
    return Opcode{0xd1, "POP DE", 1, 12, steps};
}

// JP NC, a16
Opcode Cpu::opcode_0xd2() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8;
        if (reg.flag_c) early_exit = true; });
    steps.push_back([this] { reg.pc = z16; });
    return Opcode{0xd2, "JP NC, a16", 3, 16, steps};
}

// CALL NC, a16
Opcode Cpu::opcode_0xd4() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8;
        if (reg.flag_c) early_exit = true; });
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = z16; });
    return Opcode{0xd4, "CALL NC, a16", 3, 24, steps};
}

// PUSH DE
Opcode Cpu::opcode_0xd5() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.d); });
    steps.push_back([this] { mmu->write(reg.sp, reg.e); });
    return Opcode{0xd5, "PUSH DE", 1, 16, steps};
}

// SUB d8
Opcode Cpu::opcode_0xd6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { subtract(z8); });
    return Opcode{0xd6, "SUB d8", 2, 8, steps};
}

// RST 10H
Opcode Cpu::opcode_0xd7() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = 0x10; });
    return Opcode{0xd7, "RST 10H", 1, 16, steps};
}

// RET C
Opcode Cpu::opcode_0xd8() {
    std::vector<Step> steps;
    steps.push_back([this] { if (!reg.flag_c) early_exit = true; });
    steps.push_back([this] { z8 = mmu->read(reg.sp++); });
    steps.push_back([this] { z16 = (mmu->read(reg.sp++) << 8) | z8; });
    steps.push_back([this] { reg.pc = z16; });
    return Opcode{0xd8, "RET C", 1, 20, steps};
}

// RETI
Opcode Cpu::opcode_0xd9() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.sp++); });
    steps.push_back([this] { z16 = (mmu->read(reg.sp++) << 8) | z8; });
    steps.push_back([this] { reg.pc = z16;
        interrupt_manager->enable_interrupts(); });
    return Opcode{0xd9, "RETI", 1, 16, steps};
}

// JP C, a16
Opcode Cpu::opcode_0xda() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8;
        if (!reg.flag_c) early_exit = true; });
    steps.push_back([this] { reg.pc = z16; });
    return Opcode{0xda, "JP C, a16", 3, 16, steps};
}

// CALL C, a16
Opcode Cpu::opcode_0xdc() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8;
        if (!reg.flag_c) early_exit = true; });
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = z16; });
    return Opcode{0xdc, "CALL C, a16", 3, 24, steps};
}

// SBC A, d8
Opcode Cpu::opcode_0xde() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { subtract_with_carry(z8); });
    return Opcode{0xde, "SBC A, d8", 2, 8, steps};
}

// RST 18H
Opcode Cpu::opcode_0xdf() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = 0x18; });
    return Opcode{0xdf, "RST 18H", 1, 16, steps};
}

// LDH (a8), A
Opcode Cpu::opcode_0xe0() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { mmu->write((uint16_t) (0xFF00 + z8), reg.a); });
    return Opcode{0xe0, "LDH (a8), A", 2, 12, steps};
}

// POP HL
Opcode Cpu::opcode_0xe1() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.sp++); });
    steps.push_back([this] { z16 = (mmu->read(reg.sp++) << 8) | z8; });
    steps.push_back([this] { reg.set_hl(z16); });
    return Opcode{0xe1, "POP HL", 1, 12, steps};
}

// LDH (C), A
Opcode Cpu::opcode_0xe2() {
    std::vector<Step> steps;
    steps.push_back([this] { mmu->write((uint16_t) (0xFF00 + reg.c), reg.a); });
    return Opcode{0xe2, "LDH (C), A", 1, 8, steps};
}

// PUSH HL
Opcode Cpu::opcode_0xe5() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.h); });
    steps.push_back([this] { mmu->write(reg.sp, reg.l); });
    return Opcode{0xe5, "PUSH HL", 1, 16, steps};
}

// AND d8
Opcode Cpu::opcode_0xe6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { and_(z8); });
    return Opcode{0xe6, "AND d8", 2, 8, steps};
}

// RST 20H
Opcode Cpu::opcode_0xe7() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = 0x20; });
    return Opcode{0xe7, "RST 20H", 1, 16, steps};
}

// ADD SP, r8
Opcode Cpu::opcode_0xe8() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = add_signed8(reg.sp, z8); });
    steps.push_back([this] { reg.sp = z16; });
    return Opcode{0xe8, "ADD SP, r8", 2, 16, steps};
}

// JP HL
Opcode Cpu::opcode_0xe9() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.pc = reg.hl(); });
    return Opcode{0xe9, "JP HL", 1, 4, steps};
}

// LD (a16), A
Opcode Cpu::opcode_0xea() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8; });
    steps.push_back([this] { mmu->write(z16, reg.a); });
    return Opcode{0xea, "LD (a16), A", 3, 16, steps};
}

// XOR d8
Opcode Cpu::opcode_0xee() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { xor_(z8); });
    return Opcode{0xee, "XOR d8", 2, 8, steps};
}

// RST 28H
Opcode Cpu::opcode_0xef() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = 0x28; });
    return Opcode{0xef, "RST 28H", 1, 16, steps};
}

// LDH A, (a8)
Opcode Cpu::opcode_0xf0() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z8 = mmu->read((uint16_t) (0xFF00 + z8)); });
    steps.push_back([this] { reg.a = z8; });
    return Opcode{0xf0, "LDH A, (a8)", 2, 12, steps};
}

// POP AF
Opcode Cpu::opcode_0xf1() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.sp++); });
    steps.push_back([this] { z16 = (mmu->read(reg.sp++) << 8) | z8; });
    steps.push_back([this] { reg.set_af(z16); });
    return Opcode{0xf1, "POP AF", 1, 12, steps};
}

// LDH A, (C)
Opcode Cpu::opcode_0xf2() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read((uint16_t) (0xFF00 + reg.c)); });
    steps.push_back([this] { reg.a = z8; });
    return Opcode{0xf2, "LDH A, (C)", 1, 8, steps};
}

// DI
Opcode Cpu::opcode_0xf3() {
    std::vector<Step> steps;
    steps.push_back([this] { interrupt_manager->disable_interrupts();
        interrupt_enable_scheduled = false; });
    return Opcode{0xf3, "DI", 1, 4, steps};
}

// PUSH AF
Opcode Cpu::opcode_0xf5() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.a); });
    steps.push_back([this] { mmu->write(reg.sp, reg.f()); });
    return Opcode{0xf5, "PUSH AF", 1, 16, steps};
}

// OR d8
Opcode Cpu::opcode_0xf6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { or_(z8); });
    return Opcode{0xf6, "OR d8", 2, 8, steps};
}

// RST 30H
Opcode Cpu::opcode_0xf7() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = 0x30; });
    return Opcode{0xf7, "RST 30H", 1, 16, steps};
}

// LD HL, SP+r8
Opcode Cpu::opcode_0xf8() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { reg.set_hl(add_signed8(reg.sp, z8)); });
    return Opcode{0xf8, "LD HL, SP+r8", 2, 12, steps};
}

// LD SP, HL
Opcode Cpu::opcode_0xf9() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp = reg.hl(); });
    return Opcode{0xf9, "LD SP, HL", 1, 8, steps};
}

// LD A, (a16)
Opcode Cpu::opcode_0xfa() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { z16 = (mmu->read(reg.pc++) << 8) | z8; });
    steps.push_back([this] { reg.a = mmu->read(z16); });
    return Opcode{0xfa, "LD A, (a16)", 3, 16, steps};
}

// EI
Opcode Cpu::opcode_0xfb() {
    std::vector<Step> steps;
    steps.push_back([this] { interrupt_enable_scheduled = true; });
    return Opcode{0xfb, "EI", 1, 4, steps};
}

// CP d8
Opcode Cpu::opcode_0xfe() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.pc++); });
    steps.push_back([this] { compare(z8); });
    return Opcode{0xfe, "CP d8", 2, 8, steps};
}

// RST 38H
Opcode Cpu::opcode_0xff() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.sp--; });
    steps.push_back([this] { mmu->write(reg.sp--, reg.pc >> 8); });
    steps.push_back([this] { mmu->write(reg.sp, reg.pc & 0xFF);
        reg.pc = 0x38; });
    return Opcode{0xff, "RST 38H", 1, 16, steps};
}

// RLC B
Opcode Cpu::opcode_cb_0x00() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = rotate_left_circular(reg.b); });
    return Opcode{0x00, "RLC B", 2, 8, steps};
}

// RLC C
Opcode Cpu::opcode_cb_0x01() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = rotate_left_circular(reg.c); });
    return Opcode{0x01, "RLC C", 2, 8, steps};
}

// RLC D
Opcode Cpu::opcode_cb_0x02() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = rotate_left_circular(reg.d); });
    return Opcode{0x02, "RLC D", 2, 8, steps};
}

// RLC E
Opcode Cpu::opcode_cb_0x03() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = rotate_left_circular(reg.e); });
    return Opcode{0x03, "RLC E", 2, 8, steps};
}

// RLC H
Opcode Cpu::opcode_cb_0x04() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = rotate_left_circular(reg.h); });
    return Opcode{0x04, "RLC H", 2, 8, steps};
}

// RLC L
Opcode Cpu::opcode_cb_0x05() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = rotate_left_circular(reg.l); });
    return Opcode{0x05, "RLC L", 2, 8, steps};
}

// RLC (HL)
Opcode Cpu::opcode_cb_0x06() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = rotate_left_circular(z8); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x06, "RLC (HL)", 2, 16, steps};
}

// RLC A
Opcode Cpu::opcode_cb_0x07() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = rotate_left_circular(reg.a); });
    return Opcode{0x07, "RLC A", 2, 8, steps};
}

// RRC B
Opcode Cpu::opcode_cb_0x08() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = rotate_right_circular(reg.b); });
    return Opcode{0x08, "RRC B", 2, 8, steps};
}

// RRC C
Opcode Cpu::opcode_cb_0x09() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = rotate_right_circular(reg.c); });
    return Opcode{0x09, "RRC C", 2, 8, steps};
}

// RRC D
Opcode Cpu::opcode_cb_0x0a() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = rotate_right_circular(reg.d); });
    return Opcode{0x0a, "RRC D", 2, 8, steps};
}

// RRC E
Opcode Cpu::opcode_cb_0x0b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = rotate_right_circular(reg.e); });
    return Opcode{0x0b, "RRC E", 2, 8, steps};
}

// RRC H
Opcode Cpu::opcode_cb_0x0c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = rotate_right_circular(reg.h); });
    return Opcode{0x0c, "RRC H", 2, 8, steps};
}

// RRC L
Opcode Cpu::opcode_cb_0x0d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = rotate_right_circular(reg.l); });
    return Opcode{0x0d, "RRC L", 2, 8, steps};
}

// RRC (HL)
Opcode Cpu::opcode_cb_0x0e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = rotate_right_circular(z8); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x0e, "RRC (HL)", 2, 16, steps};
}

// RRC A
Opcode Cpu::opcode_cb_0x0f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = rotate_right_circular(reg.a); });
    return Opcode{0x0f, "RRC A", 2, 8, steps};
}

// RL B
Opcode Cpu::opcode_cb_0x10() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = rotate_left(reg.b); });
    return Opcode{0x10, "RL B", 2, 8, steps};
}

// RL C
Opcode Cpu::opcode_cb_0x11() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = rotate_left(reg.c); });
    return Opcode{0x11, "RL C", 2, 8, steps};
}

// RL D
Opcode Cpu::opcode_cb_0x12() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = rotate_left(reg.d); });
    return Opcode{0x12, "RL D", 2, 8, steps};
}

// RL E
Opcode Cpu::opcode_cb_0x13() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = rotate_left(reg.e); });
    return Opcode{0x13, "RL E", 2, 8, steps};
}

// RL H
Opcode Cpu::opcode_cb_0x14() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = rotate_left(reg.h); });
    return Opcode{0x14, "RL H", 2, 8, steps};
}

// RL L
Opcode Cpu::opcode_cb_0x15() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = rotate_left(reg.l); });
    return Opcode{0x15, "RL L", 2, 8, steps};
}

// RL (HL)
Opcode Cpu::opcode_cb_0x16() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = rotate_left(z8); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x16, "RL (HL)", 2, 16, steps};
}

// RL A
Opcode Cpu::opcode_cb_0x17() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = rotate_left(reg.a); });
    return Opcode{0x17, "RL A", 2, 8, steps};
}

// RR B
Opcode Cpu::opcode_cb_0x18() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = rotate_right(reg.b); });
    return Opcode{0x18, "RR B", 2, 8, steps};
}

// RR C
Opcode Cpu::opcode_cb_0x19() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = rotate_right(reg.c); });
    return Opcode{0x19, "RR C", 2, 8, steps};
}

// RR D
Opcode Cpu::opcode_cb_0x1a() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = rotate_right(reg.d); });
    return Opcode{0x1a, "RR D", 2, 8, steps};
}

// RR E
Opcode Cpu::opcode_cb_0x1b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = rotate_right(reg.e); });
    return Opcode{0x1b, "RR E", 2, 8, steps};
}

// RR H
Opcode Cpu::opcode_cb_0x1c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = rotate_right(reg.h); });
    return Opcode{0x1c, "RR H", 2, 8, steps};
}

// RR L
Opcode Cpu::opcode_cb_0x1d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = rotate_right(reg.l); });
    return Opcode{0x1d, "RR L", 2, 8, steps};
}

// RR (HL)
Opcode Cpu::opcode_cb_0x1e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = rotate_right(z8); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x1e, "RR (HL)", 2, 16, steps};
}

// RR A
Opcode Cpu::opcode_cb_0x1f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = rotate_right(reg.a); });
    return Opcode{0x1f, "RR A", 2, 8, steps};
}

// SLA B
Opcode Cpu::opcode_cb_0x20() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = shift_left_arithmetic(reg.b); });
    return Opcode{0x20, "SLA B", 2, 8, steps};
}

// SLA C
Opcode Cpu::opcode_cb_0x21() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = shift_left_arithmetic(reg.c); });
    return Opcode{0x21, "SLA C", 2, 8, steps};
}

// SLA D
Opcode Cpu::opcode_cb_0x22() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = shift_left_arithmetic(reg.d); });
    return Opcode{0x22, "SLA D", 2, 8, steps};
}

// SLA E
Opcode Cpu::opcode_cb_0x23() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = shift_left_arithmetic(reg.e); });
    return Opcode{0x23, "SLA E", 2, 8, steps};
}

// SLA H
Opcode Cpu::opcode_cb_0x24() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = shift_left_arithmetic(reg.h); });
    return Opcode{0x24, "SLA H", 2, 8, steps};
}

// SLA L
Opcode Cpu::opcode_cb_0x25() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = shift_left_arithmetic(reg.l); });
    return Opcode{0x25, "SLA L", 2, 8, steps};
}

// SLA (HL)
Opcode Cpu::opcode_cb_0x26() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { mmu->write(reg.hl(), shift_left_arithmetic(z8)); });
    return Opcode{0x26, "SLA (HL)", 2, 16, steps};
}

// SLA A
Opcode Cpu::opcode_cb_0x27() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = shift_left_arithmetic(reg.a); });
    return Opcode{0x27, "SLA A", 2, 8, steps};
}

// SRA B
Opcode Cpu::opcode_cb_0x28() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = shift_right_arithmetic(reg.b); });
    return Opcode{0x28, "SRA B", 2, 8, steps};
}

// SRA C
Opcode Cpu::opcode_cb_0x29() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = shift_right_arithmetic(reg.c); });
    return Opcode{0x29, "SRA C", 2, 8, steps};
}

// SRA D
Opcode Cpu::opcode_cb_0x2a() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = shift_right_arithmetic(reg.d); });
    return Opcode{0x2a, "SRA D", 2, 8, steps};
}

// SRA E
Opcode Cpu::opcode_cb_0x2b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = shift_right_arithmetic(reg.e); });
    return Opcode{0x2b, "SRA E", 2, 8, steps};
}

// SRA H
Opcode Cpu::opcode_cb_0x2c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = shift_right_arithmetic(reg.h); });
    return Opcode{0x2c, "SRA H", 2, 8, steps};
}

// SRA L
Opcode Cpu::opcode_cb_0x2d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = shift_right_arithmetic(reg.l); });
    return Opcode{0x2d, "SRA L", 2, 8, steps};
}

// SRA (HL)
Opcode Cpu::opcode_cb_0x2e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { mmu->write(reg.hl(), shift_right_arithmetic(z8)); });
    return Opcode{0x2e, "SRA (HL)", 2, 16, steps};
}

// SRA A
Opcode Cpu::opcode_cb_0x2f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = shift_right_arithmetic(reg.a); });
    return Opcode{0x2f, "SRA A", 2, 8, steps};
}

// SWAP B
Opcode Cpu::opcode_cb_0x30() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = swap(reg.b); });
    return Opcode{0x30, "SWAP B", 2, 8, steps};
}

// SWAP C
Opcode Cpu::opcode_cb_0x31() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = swap(reg.c); });
    return Opcode{0x31, "SWAP C", 2, 8, steps};
}

// SWAP D
Opcode Cpu::opcode_cb_0x32() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = swap(reg.d); });
    return Opcode{0x32, "SWAP D", 2, 8, steps};
}

// SWAP E
Opcode Cpu::opcode_cb_0x33() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = swap(reg.e); });
    return Opcode{0x33, "SWAP E", 2, 8, steps};
}

// SWAP H
Opcode Cpu::opcode_cb_0x34() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = swap(reg.h); });
    return Opcode{0x34, "SWAP H", 2, 8, steps};
}

// SWAP L
Opcode Cpu::opcode_cb_0x35() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = swap(reg.l); });
    return Opcode{0x35, "SWAP L", 2, 8, steps};
}

// SWAP (HL)
Opcode Cpu::opcode_cb_0x36() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = swap(z8); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x36, "SWAP (HL)", 2, 16, steps};
}

// SWAP A
Opcode Cpu::opcode_cb_0x37() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = swap(reg.a); });
    return Opcode{0x37, "SWAP A", 2, 8, steps};
}

// SRL B
Opcode Cpu::opcode_cb_0x38() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = shift_right_logical(reg.b); });
    return Opcode{0x38, "SRL B", 2, 8, steps};
}

// SRL C
Opcode Cpu::opcode_cb_0x39() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = shift_right_logical(reg.c); });
    return Opcode{0x39, "SRL C", 2, 8, steps};
}

// SRL D
Opcode Cpu::opcode_cb_0x3a() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = shift_right_logical(reg.d); });
    return Opcode{0x3a, "SRL D", 2, 8, steps};
}

// SRL E
Opcode Cpu::opcode_cb_0x3b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = shift_right_logical(reg.e); });
    return Opcode{0x3b, "SRL E", 2, 8, steps};
}

// SRL H
Opcode Cpu::opcode_cb_0x3c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = shift_right_logical(reg.h); });
    return Opcode{0x3c, "SRL H", 2, 8, steps};
}

// SRL L
Opcode Cpu::opcode_cb_0x3d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = shift_right_logical(reg.l); });
    return Opcode{0x3d, "SRL L", 2, 8, steps};
}

// SRL (HL)
Opcode Cpu::opcode_cb_0x3e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { mmu->write(reg.hl(), shift_right_logical(z8)); });
    return Opcode{0x3e, "SRL (HL)", 2, 16, steps};
}

// SRL A
Opcode Cpu::opcode_cb_0x3f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = shift_right_logical(reg.a); });
    return Opcode{0x3f, "SRL A", 2, 8, steps};
}

// BIT 0, B
Opcode Cpu::opcode_cb_0x40() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.b, 0); });
    return Opcode{0x40, "BIT 0, B", 2, 8, steps};
}

// BIT 0, C
Opcode Cpu::opcode_cb_0x41() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.c, 0); });
    return Opcode{0x41, "BIT 0, C", 2, 8, steps};
}

// BIT 0, D
Opcode Cpu::opcode_cb_0x42() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.d, 0); });
    return Opcode{0x42, "BIT 0, D", 2, 8, steps};
}

// BIT 0, E
Opcode Cpu::opcode_cb_0x43() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.e, 0); });
    return Opcode{0x43, "BIT 0, E", 2, 8, steps};
}

// BIT 0, H
Opcode Cpu::opcode_cb_0x44() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.h, 0); });
    return Opcode{0x44, "BIT 0, H", 2, 8, steps};
}

// BIT 0, L
Opcode Cpu::opcode_cb_0x45() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.l, 0); });
    return Opcode{0x45, "BIT 0, L", 2, 8, steps};
}

// BIT 0, (HL)
Opcode Cpu::opcode_cb_0x46() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { bit(z8, 0); });
    return Opcode{0x46, "BIT 0, (HL)", 2, 12, steps};
}

// BIT 0, A
Opcode Cpu::opcode_cb_0x47() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.a, 0); });
    return Opcode{0x47, "BIT 0, A", 2, 8, steps};
}

// BIT 1, B
Opcode Cpu::opcode_cb_0x48() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.b, 1); });
    return Opcode{0x48, "BIT 1, B", 2, 8, steps};
}

// BIT 1, C
Opcode Cpu::opcode_cb_0x49() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.c, 1); });
    return Opcode{0x49, "BIT 1, C", 2, 8, steps};
}

// BIT 1, D
Opcode Cpu::opcode_cb_0x4a() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.d, 1); });
    return Opcode{0x4a, "BIT 1, D", 2, 8, steps};
}

// BIT 1, E
Opcode Cpu::opcode_cb_0x4b() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.e, 1); });
    return Opcode{0x4b, "BIT 1, E", 2, 8, steps};
}

// BIT 1, H
Opcode Cpu::opcode_cb_0x4c() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.h, 1); });
    return Opcode{0x4c, "BIT 1, H", 2, 8, steps};
}

// BIT 1, L
Opcode Cpu::opcode_cb_0x4d() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.l, 1); });
    return Opcode{0x4d, "BIT 1, L", 2, 8, steps};
}

// BIT 1, (HL)
Opcode Cpu::opcode_cb_0x4e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { bit(z8, 1); });
    return Opcode{0x4e, "BIT 1, (HL)", 2, 12, steps};
}

// BIT 1, A
Opcode Cpu::opcode_cb_0x4f() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.a, 1); });
    return Opcode{0x4f, "BIT 1, A", 2, 8, steps};
}

// BIT 2, B
Opcode Cpu::opcode_cb_0x50() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.b, 2); });
    return Opcode{0x50, "BIT 2, B", 2, 8, steps};
}

// BIT 2, C
Opcode Cpu::opcode_cb_0x51() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.c, 2); });
    return Opcode{0x51, "BIT 2, C", 2, 8, steps};
}

// BIT 2, D
Opcode Cpu::opcode_cb_0x52() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.d, 2); });
    return Opcode{0x52, "BIT 2, D", 2, 8, steps};
}

// BIT 2, E
Opcode Cpu::opcode_cb_0x53() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.e, 2); });
    return Opcode{0x53, "BIT 2, E", 2, 8, steps};
}

// BIT 2, H
Opcode Cpu::opcode_cb_0x54() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.h, 2); });
    return Opcode{0x54, "BIT 2, H", 2, 8, steps};
}

// BIT 2, L
Opcode Cpu::opcode_cb_0x55() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.l, 2); });
    return Opcode{0x55, "BIT 2, L", 2, 8, steps};
}

// BIT 2, (HL)
Opcode Cpu::opcode_cb_0x56() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { bit(z8, 2); });
    return Opcode{0x56, "BIT 2, (HL)", 2, 12, steps};
}

// BIT 2, A
Opcode Cpu::opcode_cb_0x57() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.a, 2); });
    return Opcode{0x57, "BIT 2, A", 2, 8, steps};
}

// BIT 3, B
Opcode Cpu::opcode_cb_0x58() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.b, 3); });
    return Opcode{0x58, "BIT 3, B", 2, 8, steps};
}

// BIT 3, C
Opcode Cpu::opcode_cb_0x59() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.c, 3); });
    return Opcode{0x59, "BIT 3, C", 2, 8, steps};
}

// BIT 3, D
Opcode Cpu::opcode_cb_0x5a() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.d, 3); });
    return Opcode{0x5a, "BIT 3, D", 2, 8, steps};
}

// BIT 3, E
Opcode Cpu::opcode_cb_0x5b() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.e, 3); });
    return Opcode{0x5b, "BIT 3, E", 2, 8, steps};
}

// BIT 3, H
Opcode Cpu::opcode_cb_0x5c() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.h, 3); });
    return Opcode{0x5c, "BIT 3, H", 2, 8, steps};
}

// BIT 3, L
Opcode Cpu::opcode_cb_0x5d() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.l, 3); });
    return Opcode{0x5d, "BIT 3, L", 2, 8, steps};
}

// BIT 3, (HL)
Opcode Cpu::opcode_cb_0x5e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { bit(z8, 3); });
    return Opcode{0x5e, "BIT 3, (HL)", 2, 12, steps};
}

// BIT 3, A
Opcode Cpu::opcode_cb_0x5f() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.a, 3); });
    return Opcode{0x5f, "BIT 3, A", 2, 8, steps};
}

// BIT 4, B
Opcode Cpu::opcode_cb_0x60() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.b, 4); });
    return Opcode{0x60, "BIT 4, B", 2, 8, steps};
}

// BIT 4, C
Opcode Cpu::opcode_cb_0x61() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.c, 4); });
    return Opcode{0x61, "BIT 4, C", 2, 8, steps};
}

// BIT 4, D
Opcode Cpu::opcode_cb_0x62() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.d, 4); });
    return Opcode{0x62, "BIT 4, D", 2, 8, steps};
}

// BIT 4, E
Opcode Cpu::opcode_cb_0x63() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.e, 4); });
    return Opcode{0x63, "BIT 4, E", 2, 8, steps};
}

// BIT 4, H
Opcode Cpu::opcode_cb_0x64() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.h, 4); });
    return Opcode{0x64, "BIT 4, H", 2, 8, steps};
}

// BIT 4, L
Opcode Cpu::opcode_cb_0x65() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.l, 4); });
    return Opcode{0x65, "BIT 4, L", 2, 8, steps};
}

// BIT 4, (HL)
Opcode Cpu::opcode_cb_0x66() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { bit(z8, 4); });
    return Opcode{0x66, "BIT 4, (HL)", 2, 12, steps};
}

// BIT 4, A
Opcode Cpu::opcode_cb_0x67() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.a, 4); });
    return Opcode{0x67, "BIT 4, A", 2, 8, steps};
}

// BIT 5, B
Opcode Cpu::opcode_cb_0x68() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.b, 5); });
    return Opcode{0x68, "BIT 5, B", 2, 8, steps};
}

// BIT 5, C
Opcode Cpu::opcode_cb_0x69() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.c, 5); });
    return Opcode{0x69, "BIT 5, C", 2, 8, steps};
}

// BIT 5, D
Opcode Cpu::opcode_cb_0x6a() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.d, 5); });
    return Opcode{0x6a, "BIT 5, D", 2, 8, steps};
}

// BIT 5, E
Opcode Cpu::opcode_cb_0x6b() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.e, 5); });
    return Opcode{0x6b, "BIT 5, E", 2, 8, steps};
}

// BIT 5, H
Opcode Cpu::opcode_cb_0x6c() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.h, 5); });
    return Opcode{0x6c, "BIT 5, H", 2, 8, steps};
}

// BIT 5, L
Opcode Cpu::opcode_cb_0x6d() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.l, 5); });
    return Opcode{0x6d, "BIT 5, L", 2, 8, steps};
}

// BIT 5, (HL)
Opcode Cpu::opcode_cb_0x6e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { bit(z8, 5); });
    return Opcode{0x6e, "BIT 5, (HL)", 2, 12, steps};
}

// BIT 5, A
Opcode Cpu::opcode_cb_0x6f() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.a, 5); });
    return Opcode{0x6f, "BIT 5, A", 2, 8, steps};
}

// BIT 6, B
Opcode Cpu::opcode_cb_0x70() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.b, 6); });
    return Opcode{0x70, "BIT 6, B", 2, 8, steps};
}

// BIT 6, C
Opcode Cpu::opcode_cb_0x71() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.c, 6); });
    return Opcode{0x71, "BIT 6, C", 2, 8, steps};
}

// BIT 6, D
Opcode Cpu::opcode_cb_0x72() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.d, 6); });
    return Opcode{0x72, "BIT 6, D", 2, 8, steps};
}

// BIT 6, E
Opcode Cpu::opcode_cb_0x73() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.e, 6); });
    return Opcode{0x73, "BIT 6, E", 2, 8, steps};
}

// BIT 6, H
Opcode Cpu::opcode_cb_0x74() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.h, 6); });
    return Opcode{0x74, "BIT 6, H", 2, 8, steps};
}

// BIT 6, L
Opcode Cpu::opcode_cb_0x75() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.l, 6); });
    return Opcode{0x75, "BIT 6, L", 2, 8, steps};
}

// BIT 6, (HL)
Opcode Cpu::opcode_cb_0x76() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { bit(z8, 6); });
    return Opcode{0x76, "BIT 6, (HL)", 2, 12, steps};
}

// BIT 6, A
Opcode Cpu::opcode_cb_0x77() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.a, 6); });
    return Opcode{0x77, "BIT 6, A", 2, 8, steps};
}

// BIT 7, B
Opcode Cpu::opcode_cb_0x78() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.b, 7); });
    return Opcode{0x78, "BIT 7, B", 2, 8, steps};
}

// BIT 7, C
Opcode Cpu::opcode_cb_0x79() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.c, 7); });
    return Opcode{0x79, "BIT 7, C", 2, 8, steps};
}

// BIT 7, D
Opcode Cpu::opcode_cb_0x7a() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.d, 7); });
    return Opcode{0x7a, "BIT 7, D", 2, 8, steps};
}

// BIT 7, E
Opcode Cpu::opcode_cb_0x7b() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.e, 7); });
    return Opcode{0x7b, "BIT 7, E", 2, 8, steps};
}

// BIT 7, H
Opcode Cpu::opcode_cb_0x7c() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.h, 7); });
    return Opcode{0x7c, "BIT 7, H", 2, 8, steps};
}

// BIT 7, L
Opcode Cpu::opcode_cb_0x7d() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.l, 7); });
    return Opcode{0x7d, "BIT 7, L", 2, 8, steps};
}

// BIT 7, (HL)
Opcode Cpu::opcode_cb_0x7e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { bit(z8, 7); });
    return Opcode{0x7e, "BIT 7, (HL)", 2, 12, steps};
}

// BIT 7, A
Opcode Cpu::opcode_cb_0x7f() {
    std::vector<Step> steps;
    steps.push_back([this] { bit(reg.a, 7); });
    return Opcode{0x7f, "BIT 7, A", 2, 8, steps};
}

// RES 0, B
Opcode Cpu::opcode_cb_0x80() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reset(reg.b, 0); });
    return Opcode{0x80, "RES 0, B", 2, 8, steps};
}

// RES 0, C
Opcode Cpu::opcode_cb_0x81() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reset(reg.c, 0); });
    return Opcode{0x81, "RES 0, C", 2, 8, steps};
}

// RES 0, D
Opcode Cpu::opcode_cb_0x82() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reset(reg.d, 0); });
    return Opcode{0x82, "RES 0, D", 2, 8, steps};
}

// RES 0, E
Opcode Cpu::opcode_cb_0x83() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reset(reg.e, 0); });
    return Opcode{0x83, "RES 0, E", 2, 8, steps};
}

// RES 0, H
Opcode Cpu::opcode_cb_0x84() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reset(reg.h, 0); });
    return Opcode{0x84, "RES 0, H", 2, 8, steps};
}

// RES 0, L
Opcode Cpu::opcode_cb_0x85() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reset(reg.l, 0); });
    return Opcode{0x85, "RES 0, L", 2, 8, steps};
}

// RES 0, (HL)
Opcode Cpu::opcode_cb_0x86() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = reset(z8, 0); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x86, "RES 0, (HL)", 2, 16, steps};
}

// RES 0, A
Opcode Cpu::opcode_cb_0x87() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reset(reg.a, 0); });
    return Opcode{0x87, "RES 0, A", 2, 8, steps};
}

// RES 1, B
Opcode Cpu::opcode_cb_0x88() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reset(reg.b, 1); });
    return Opcode{0x88, "RES 1, B", 2, 8, steps};
}

// RES 1, C
Opcode Cpu::opcode_cb_0x89() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reset(reg.c, 1); });
    return Opcode{0x89, "RES 1, C", 2, 8, steps};
}

// RES 1, D
Opcode Cpu::opcode_cb_0x8a() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reset(reg.d, 1); });
    return Opcode{0x8a, "RES 1, D", 2, 8, steps};
}

// RES 1, E
Opcode Cpu::opcode_cb_0x8b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reset(reg.e, 1); });
    return Opcode{0x8b, "RES 1, E", 2, 8, steps};
}

// RES 1, H
Opcode Cpu::opcode_cb_0x8c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reset(reg.h, 1); });
    return Opcode{0x8c, "RES 1, H", 2, 8, steps};
}

// RES 1, L
Opcode Cpu::opcode_cb_0x8d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reset(reg.l, 1); });
    return Opcode{0x8d, "RES 1, L", 2, 8, steps};
}

// RES 1, (HL)
Opcode Cpu::opcode_cb_0x8e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = reset(z8, 1); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x8e, "RES 1, (HL)", 2, 16, steps};
}

// RES 1, A
Opcode Cpu::opcode_cb_0x8f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reset(reg.a, 1); });
    return Opcode{0x8f, "RES 1, A", 2, 8, steps};
}

// RES 2, B
Opcode Cpu::opcode_cb_0x90() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reset(reg.b, 2); });
    return Opcode{0x90, "RES 2, B", 2, 8, steps};
}

// RES 2, C
Opcode Cpu::opcode_cb_0x91() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reset(reg.c, 2); });
    return Opcode{0x91, "RES 2, C", 2, 8, steps};
}

// RES 2, D
Opcode Cpu::opcode_cb_0x92() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reset(reg.d, 2); });
    return Opcode{0x92, "RES 2, D", 2, 8, steps};
}

// RES 2, E
Opcode Cpu::opcode_cb_0x93() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reset(reg.e, 2); });
    return Opcode{0x93, "RES 2, E", 2, 8, steps};
}

// RES 2, H
Opcode Cpu::opcode_cb_0x94() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reset(reg.h, 2); });
    return Opcode{0x94, "RES 2, H", 2, 8, steps};
}

// RES 2, L
Opcode Cpu::opcode_cb_0x95() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reset(reg.l, 2); });
    return Opcode{0x95, "RES 2, L", 2, 8, steps};
}

// RES 2, (HL)
Opcode Cpu::opcode_cb_0x96() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = reset(z8, 2); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x96, "RES 2, (HL)", 2, 16, steps};
}

// RES 2, A
Opcode Cpu::opcode_cb_0x97() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reset(reg.a, 2); });
    return Opcode{0x97, "RES 2, A", 2, 8, steps};
}

// RES 3, B
Opcode Cpu::opcode_cb_0x98() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reset(reg.b, 3); });
    return Opcode{0x98, "RES 3, B", 2, 8, steps};
}

// RES 3, C
Opcode Cpu::opcode_cb_0x99() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reset(reg.c, 3); });
    return Opcode{0x99, "RES 3, C", 2, 8, steps};
}

// RES 3, D
Opcode Cpu::opcode_cb_0x9a() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reset(reg.d, 3); });
    return Opcode{0x9a, "RES 3, D", 2, 8, steps};
}

// RES 3, E
Opcode Cpu::opcode_cb_0x9b() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reset(reg.e, 3); });
    return Opcode{0x9b, "RES 3, E", 2, 8, steps};
}

// RES 3, H
Opcode Cpu::opcode_cb_0x9c() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reset(reg.h, 3); });
    return Opcode{0x9c, "RES 3, H", 2, 8, steps};
}

// RES 3, L
Opcode Cpu::opcode_cb_0x9d() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reset(reg.l, 3); });
    return Opcode{0x9d, "RES 3, L", 2, 8, steps};
}

// RES 3, (HL)
Opcode Cpu::opcode_cb_0x9e() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = reset(z8, 3); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0x9e, "RES 3, (HL)", 2, 16, steps};
}

// RES 3, A
Opcode Cpu::opcode_cb_0x9f() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reset(reg.a, 3); });
    return Opcode{0x9f, "RES 3, A", 2, 8, steps};
}

// RES 4, B
Opcode Cpu::opcode_cb_0xa0() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reset(reg.b, 4); });
    return Opcode{0xa0, "RES 4, B", 2, 8, steps};
}

// RES 4, C
Opcode Cpu::opcode_cb_0xa1() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reset(reg.c, 4); });
    return Opcode{0xa1, "RES 4, C", 2, 8, steps};
}

// RES 4, D
Opcode Cpu::opcode_cb_0xa2() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reset(reg.d, 4); });
    return Opcode{0xa2, "RES 4, D", 2, 8, steps};
}

// RES 4, E
Opcode Cpu::opcode_cb_0xa3() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reset(reg.e, 4); });
    return Opcode{0xa3, "RES 4, E", 2, 8, steps};
}

// RES 4, H
Opcode Cpu::opcode_cb_0xa4() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reset(reg.h, 4); });
    return Opcode{0xa4, "RES 4, H", 2, 8, steps};
}

// RES 4, L
Opcode Cpu::opcode_cb_0xa5() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reset(reg.l, 4); });
    return Opcode{0xa5, "RES 4, L", 2, 8, steps};
}

// RES 4, (HL)
Opcode Cpu::opcode_cb_0xa6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = reset(z8, 4); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xa6, "RES 4, (HL)", 2, 16, steps};
}

// RES 4, A
Opcode Cpu::opcode_cb_0xa7() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reset(reg.a, 4); });
    return Opcode{0xa7, "RES 4, A", 2, 8, steps};
}

// RES 5, B
Opcode Cpu::opcode_cb_0xa8() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reset(reg.b, 5); });
    return Opcode{0xa8, "RES 5, B", 2, 8, steps};
}

// RES 5, C
Opcode Cpu::opcode_cb_0xa9() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reset(reg.c, 5); });
    return Opcode{0xa9, "RES 5, C", 2, 8, steps};
}

// RES 5, D
Opcode Cpu::opcode_cb_0xaa() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reset(reg.d, 5); });
    return Opcode{0xaa, "RES 5, D", 2, 8, steps};
}

// RES 5, E
Opcode Cpu::opcode_cb_0xab() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reset(reg.e, 5); });
    return Opcode{0xab, "RES 5, E", 2, 8, steps};
}

// RES 5, H
Opcode Cpu::opcode_cb_0xac() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reset(reg.h, 5); });
    return Opcode{0xac, "RES 5, H", 2, 8, steps};
}

// RES 5, L
Opcode Cpu::opcode_cb_0xad() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reset(reg.l, 5); });
    return Opcode{0xad, "RES 5, L", 2, 8, steps};
}

// RES 5, (HL)
Opcode Cpu::opcode_cb_0xae() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = reset(z8, 5); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xae, "RES 5, (HL)", 2, 16, steps};
}

// RES 5, A
Opcode Cpu::opcode_cb_0xaf() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reset(reg.a, 5); });
    return Opcode{0xaf, "RES 5, A", 2, 8, steps};
}

// RES 6, B
Opcode Cpu::opcode_cb_0xb0() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reset(reg.b, 6); });
    return Opcode{0xb0, "RES 6, B", 2, 8, steps};
}

// RES 6, C
Opcode Cpu::opcode_cb_0xb1() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reset(reg.c, 6); });
    return Opcode{0xb1, "RES 6, C", 2, 8, steps};
}

// RES 6, D
Opcode Cpu::opcode_cb_0xb2() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reset(reg.d, 6); });
    return Opcode{0xb2, "RES 6, D", 2, 8, steps};
}

// RES 6, E
Opcode Cpu::opcode_cb_0xb3() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reset(reg.e, 6); });
    return Opcode{0xb3, "RES 6, E", 2, 8, steps};
}

// RES 6, H
Opcode Cpu::opcode_cb_0xb4() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reset(reg.h, 6); });
    return Opcode{0xb4, "RES 6, H", 2, 8, steps};
}

// RES 6, L
Opcode Cpu::opcode_cb_0xb5() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reset(reg.l, 6); });
    return Opcode{0xb5, "RES 6, L", 2, 8, steps};
}

// RES 6, (HL)
Opcode Cpu::opcode_cb_0xb6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = reset(z8, 6); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xb6, "RES 6, (HL)", 2, 16, steps};
}

// RES 6, A
Opcode Cpu::opcode_cb_0xb7() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reset(reg.a, 6); });
    return Opcode{0xb7, "RES 6, A", 2, 8, steps};
}

// RES 7, B
Opcode Cpu::opcode_cb_0xb8() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = reset(reg.b, 7); });
    return Opcode{0xb8, "RES 7, B", 2, 8, steps};
}

// RES 7, C
Opcode Cpu::opcode_cb_0xb9() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = reset(reg.c, 7); });
    return Opcode{0xb9, "RES 7, C", 2, 8, steps};
}

// RES 7, D
Opcode Cpu::opcode_cb_0xba() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = reset(reg.d, 7); });
    return Opcode{0xba, "RES 7, D", 2, 8, steps};
}

// RES 7, E
Opcode Cpu::opcode_cb_0xbb() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = reset(reg.e, 7); });
    return Opcode{0xbb, "RES 7, E", 2, 8, steps};
}

// RES 7, H
Opcode Cpu::opcode_cb_0xbc() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = reset(reg.h, 7); });
    return Opcode{0xbc, "RES 7, H", 2, 8, steps};
}

// RES 7, L
Opcode Cpu::opcode_cb_0xbd() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = reset(reg.l, 7); });
    return Opcode{0xbd, "RES 7, L", 2, 8, steps};
}

// RES 7, (HL)
Opcode Cpu::opcode_cb_0xbe() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = reset(z8, 7); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xbe, "RES 7, (HL)", 2, 16, steps};
}

// RES 7, A
Opcode Cpu::opcode_cb_0xbf() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = reset(reg.a, 7); });
    return Opcode{0xbf, "RES 7, A", 2, 8, steps};
}

// SET 0, B
Opcode Cpu::opcode_cb_0xc0() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = set(reg.b, 0); });
    return Opcode{0xc0, "SET 0, B", 2, 8, steps};
}

// SET 0, C
Opcode Cpu::opcode_cb_0xc1() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = set(reg.c, 0); });
    return Opcode{0xc1, "SET 0, C", 2, 8, steps};
}

// SET 0, D
Opcode Cpu::opcode_cb_0xc2() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = set(reg.d, 0); });
    return Opcode{0xc2, "SET 0, D", 2, 8, steps};
}

// SET 0, E
Opcode Cpu::opcode_cb_0xc3() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = set(reg.e, 0); });
    return Opcode{0xc3, "SET 0, E", 2, 8, steps};
}

// SET 0, H
Opcode Cpu::opcode_cb_0xc4() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = set(reg.h, 0); });
    return Opcode{0xc4, "SET 0, H", 2, 8, steps};
}

// SET 0, L
Opcode Cpu::opcode_cb_0xc5() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = set(reg.l, 0); });
    return Opcode{0xc5, "SET 0, L", 2, 8, steps};
}

// SET 0, (HL)
Opcode Cpu::opcode_cb_0xc6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = set(z8, 0); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xc6, "SET 0, (HL)", 2, 16, steps};
}

// SET 0, A
Opcode Cpu::opcode_cb_0xc7() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = set(reg.a, 0); });
    return Opcode{0xc7, "SET 0, A", 2, 8, steps};
}

// SET 1, B
Opcode Cpu::opcode_cb_0xc8() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = set(reg.b, 1); });
    return Opcode{0xc8, "SET 1, B", 2, 8, steps};
}

// SET 1, C
Opcode Cpu::opcode_cb_0xc9() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = set(reg.c, 1); });
    return Opcode{0xc9, "SET 1, C", 2, 8, steps};
}

// SET 1, D
Opcode Cpu::opcode_cb_0xca() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = set(reg.d, 1); });
    return Opcode{0xca, "SET 1, D", 2, 8, steps};
}

// SET 1, E
Opcode Cpu::opcode_cb_0xcb() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = set(reg.e, 1); });
    return Opcode{0xcb, "SET 1, E", 2, 8, steps};
}

// SET 1, H
Opcode Cpu::opcode_cb_0xcc() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = set(reg.h, 1); });
    return Opcode{0xcc, "SET 1, H", 2, 8, steps};
}

// SET 1, L
Opcode Cpu::opcode_cb_0xcd() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = set(reg.l, 1); });
    return Opcode{0xcd, "SET 1, L", 2, 8, steps};
}

// SET 1, (HL)
Opcode Cpu::opcode_cb_0xce() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = set(z8, 1); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xce, "SET 1, (HL)", 2, 16, steps};
}

// SET 1, A
Opcode Cpu::opcode_cb_0xcf() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = set(reg.a, 1); });
    return Opcode{0xcf, "SET 1, A", 2, 8, steps};
}

// SET 2, B
Opcode Cpu::opcode_cb_0xd0() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = set(reg.b, 2); });
    return Opcode{0xd0, "SET 2, B", 2, 8, steps};
}

// SET 2, C
Opcode Cpu::opcode_cb_0xd1() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = set(reg.c, 2); });
    return Opcode{0xd1, "SET 2, C", 2, 8, steps};
}

// SET 2, D
Opcode Cpu::opcode_cb_0xd2() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = set(reg.d, 2); });
    return Opcode{0xd2, "SET 2, D", 2, 8, steps};
}

// SET 2, E
Opcode Cpu::opcode_cb_0xd3() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = set(reg.e, 2); });
    return Opcode{0xd3, "SET 2, E", 2, 8, steps};
}

// SET 2, H
Opcode Cpu::opcode_cb_0xd4() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = set(reg.h, 2); });
    return Opcode{0xd4, "SET 2, H", 2, 8, steps};
}

// SET 2, L
Opcode Cpu::opcode_cb_0xd5() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = set(reg.l, 2); });
    return Opcode{0xd5, "SET 2, L", 2, 8, steps};
}

// SET 2, (HL)
Opcode Cpu::opcode_cb_0xd6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = set(z8, 2); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xd6, "SET 2, (HL)", 2, 16, steps};
}

// SET 2, A
Opcode Cpu::opcode_cb_0xd7() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = set(reg.a, 2); });
    return Opcode{0xd7, "SET 2, A", 2, 8, steps};
}

// SET 3, B
Opcode Cpu::opcode_cb_0xd8() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = set(reg.b, 3); });
    return Opcode{0xd8, "SET 3, B", 2, 8, steps};
}

// SET 3, C
Opcode Cpu::opcode_cb_0xd9() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = set(reg.c, 3); });
    return Opcode{0xd9, "SET 3, C", 2, 8, steps};
}

// SET 3, D
Opcode Cpu::opcode_cb_0xda() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = set(reg.d, 3); });
    return Opcode{0xda, "SET 3, D", 2, 8, steps};
}

// SET 3, E
Opcode Cpu::opcode_cb_0xdb() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = set(reg.e, 3); });
    return Opcode{0xdb, "SET 3, E", 2, 8, steps};
}

// SET 3, H
Opcode Cpu::opcode_cb_0xdc() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = set(reg.h, 3); });
    return Opcode{0xdc, "SET 3, H", 2, 8, steps};
}

// SET 3, L
Opcode Cpu::opcode_cb_0xdd() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = set(reg.l, 3); });
    return Opcode{0xdd, "SET 3, L", 2, 8, steps};
}

// SET 3, (HL)
Opcode Cpu::opcode_cb_0xde() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = set(z8, 3); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xde, "SET 3, (HL)", 2, 16, steps};
}

// SET 3, A
Opcode Cpu::opcode_cb_0xdf() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = set(reg.a, 3); });
    return Opcode{0xdf, "SET 3, A", 2, 8, steps};
}

// SET 4, B
Opcode Cpu::opcode_cb_0xe0() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = set(reg.b, 4); });
    return Opcode{0xe0, "SET 4, B", 2, 8, steps};
}

// SET 4, C
Opcode Cpu::opcode_cb_0xe1() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = set(reg.c, 4); });
    return Opcode{0xe1, "SET 4, C", 2, 8, steps};
}

// SET 4, D
Opcode Cpu::opcode_cb_0xe2() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = set(reg.d, 4); });
    return Opcode{0xe2, "SET 4, D", 2, 8, steps};
}

// SET 4, E
Opcode Cpu::opcode_cb_0xe3() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = set(reg.e, 4); });
    return Opcode{0xe3, "SET 4, E", 2, 8, steps};
}

// SET 4, H
Opcode Cpu::opcode_cb_0xe4() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = set(reg.h, 4); });
    return Opcode{0xe4, "SET 4, H", 2, 8, steps};
}

// SET 4, L
Opcode Cpu::opcode_cb_0xe5() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = set(reg.l, 4); });
    return Opcode{0xe5, "SET 4, L", 2, 8, steps};
}

// SET 4, (HL)
Opcode Cpu::opcode_cb_0xe6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = set(z8, 4); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xe6, "SET 4, (HL)", 2, 16, steps};
}

// SET 4, A
Opcode Cpu::opcode_cb_0xe7() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = set(reg.a, 4); });
    return Opcode{0xe7, "SET 4, A", 2, 8, steps};
}

// SET 5, B
Opcode Cpu::opcode_cb_0xe8() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = set(reg.b, 5); });
    return Opcode{0xe8, "SET 5, B", 2, 8, steps};
}

// SET 5, C
Opcode Cpu::opcode_cb_0xe9() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = set(reg.c, 5); });
    return Opcode{0xe9, "SET 5, C", 2, 8, steps};
}

// SET 5, D
Opcode Cpu::opcode_cb_0xea() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = set(reg.d, 5); });
    return Opcode{0xea, "SET 5, D", 2, 8, steps};
}

// SET 5, E
Opcode Cpu::opcode_cb_0xeb() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = set(reg.e, 5); });
    return Opcode{0xeb, "SET 5, E", 2, 8, steps};
}

// SET 5, H
Opcode Cpu::opcode_cb_0xec() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = set(reg.h, 5); });
    return Opcode{0xec, "SET 5, H", 2, 8, steps};
}

// SET 5, L
Opcode Cpu::opcode_cb_0xed() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = set(reg.l, 5); });
    return Opcode{0xed, "SET 5, L", 2, 8, steps};
}

// SET 5, (HL)
Opcode Cpu::opcode_cb_0xee() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = set(z8, 5); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xee, "SET 5, (HL)", 2, 16, steps};
}

// SET 5, A
Opcode Cpu::opcode_cb_0xef() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = set(reg.a, 5); });
    return Opcode{0xef, "SET 5, A", 2, 8, steps};
}

// SET 6, B
Opcode Cpu::opcode_cb_0xf0() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = set(reg.b, 6); });
    return Opcode{0xf0, "SET 6, B", 2, 8, steps};
}

// SET 6, C
Opcode Cpu::opcode_cb_0xf1() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = set(reg.c, 6); });
    return Opcode{0xf1, "SET 6, C", 2, 8, steps};
}

// SET 6, D
Opcode Cpu::opcode_cb_0xf2() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = set(reg.d, 6); });
    return Opcode{0xf2, "SET 6, D", 2, 8, steps};
}

// SET 6, E
Opcode Cpu::opcode_cb_0xf3() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = set(reg.e, 6); });
    return Opcode{0xf3, "SET 6, E", 2, 8, steps};
}

// SET 6, H
Opcode Cpu::opcode_cb_0xf4() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = set(reg.h, 6); });
    return Opcode{0xf4, "SET 6, H", 2, 8, steps};
}

// SET 6, L
Opcode Cpu::opcode_cb_0xf5() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = set(reg.l, 6); });
    return Opcode{0xf5, "SET 6, L", 2, 8, steps};
}

// SET 6, (HL)
Opcode Cpu::opcode_cb_0xf6() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = set(z8, 6); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xf6, "SET 6, (HL)", 2, 16, steps};
}

// SET 6, A
Opcode Cpu::opcode_cb_0xf7() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = set(reg.a, 6); });
    return Opcode{0xf7, "SET 6, A", 2, 8, steps};
}

// SET 7, B
Opcode Cpu::opcode_cb_0xf8() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.b = set(reg.b, 7); });
    return Opcode{0xf8, "SET 7, B", 2, 8, steps};
}

// SET 7, C
Opcode Cpu::opcode_cb_0xf9() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.c = set(reg.c, 7); });
    return Opcode{0xf9, "SET 7, C", 2, 8, steps};
}

// SET 7, D
Opcode Cpu::opcode_cb_0xfa() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.d = set(reg.d, 7); });
    return Opcode{0xfa, "SET 7, D", 2, 8, steps};
}

// SET 7, E
Opcode Cpu::opcode_cb_0xfb() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.e = set(reg.e, 7); });
    return Opcode{0xfb, "SET 7, E", 2, 8, steps};
}

// SET 7, H
Opcode Cpu::opcode_cb_0xfc() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.h = set(reg.h, 7); });
    return Opcode{0xfc, "SET 7, H", 2, 8, steps};
}

// SET 7, L
Opcode Cpu::opcode_cb_0xfd() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.l = set(reg.l, 7); });
    return Opcode{0xfd, "SET 7, L", 2, 8, steps};
}

// SET 7, (HL)
Opcode Cpu::opcode_cb_0xfe() {
    std::vector<Step> steps;
    steps.push_back([this] { z8 = mmu->read(reg.hl()); });
    steps.push_back([this] { z8 = set(z8, 7); });
    steps.push_back([this] { mmu->write(reg.hl(), z8); });
    return Opcode{0xfe, "SET 7, (HL)", 2, 16, steps};
}

// SET 7, A
Opcode Cpu::opcode_cb_0xff() {
    std::vector<Step> steps;
    steps.push_back([this] { reg.a = set(reg.a, 7); });
    return Opcode{0xff, "SET 7, A", 2, 8, steps};
}

