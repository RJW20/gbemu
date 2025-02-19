#include <stdexcept>
#include <string>
#include <sstream>
#include <bitset>
#include "../logger.hpp"
#include "cpu.hpp"
#include "../interrupt_manager.hpp"

/* Initialise all the Opcodes.
 * Reset the CPU to post boot ROM state. */
Cpu::Cpu(InterruptManager* interrupt_manager, Mmu* mmu) :
    interrupt_manager(interrupt_manager), mmu(mmu) {
    initialise_opcodes_dictionaries();
    reset();
}

/* Set the Register to its post boot ROM state.
 * Set state to Fetch. */
void Cpu::reset() {
    reg.reset();
    locked = 0;
    set_state(State::FETCH);
    halt_bug = false;
    opcode = &(opcodes[0]);
    interrupt_enable_scheduled = false;
}

// Carry out 1 t-cycle.
void Cpu::tick() {

    // Only act every 4 t-cycles
    if (++locked < 4) {
        return;
    }
    locked = 0;

    switch(state) {

        case State::FETCH:
            fetch_cycle();
            break;

        case State::WORK:
            work_cycle();
            break;

        case State::INTERRUPT:
            interrupt_cycle();
            break;

        case State::HALT:
            halt_cycle();
            break;

        case State::STOP:
            return;
    }
}

/* Set the CPU state to the given State. 
 * Resets the appropriate variables in preparation for the new State. */
void Cpu::set_state(State new_state) {
    state = new_state;
    switch(state) {
        case State::FETCH:
            cb_prefix = false;
            break;
        case State::WORK:
            current_m_cycles = 0;
            early_exit = false;
            break;
        case State::INTERRUPT:
            current_m_cycles = 0;
            break;
    }
}

/* Carry out 1 FETCH m-cycle.
 * Reads the address at the current reg.pc and increments it.
 * Sets cb_prefix or the current opcode and state as necessary. */
void Cpu::fetch_cycle() {

    uint8_t opcode_address = mmu->read(reg.pc++);

    if (!cb_prefix){
        switch(opcode_address) {
            case 0xCB:
                cb_prefix = true;
                return;
            case 0x76:
                if (!halt_bug) {
                    state = State::HALT;
                }
                else {
                    state == State::FETCH;
                    halt_bug = false;
                }
                return;
            case 0x10:
                state = State::STOP;
                return;
        }
    }

    opcode = cb_prefix ?
        &(cb_opcodes[opcode_address]) : &(opcodes[opcode_address]);
    opcode_m_cycles = cb_prefix ?
        opcode->t_cycles / 4 - 1 : opcode->t_cycles / 4;

    Log::debug(*this);

    set_state(State::WORK);
}

/* Carry out 1 WORK m-cycle.
 * Executes the opcode.step at the current_m_cycle and increments it.
 * Sets to FETCH state if exits early.
 * Carries out an additional fetch_cycle if on the last step of the opcode. */
void Cpu::work_cycle() {

    // Opcodes sometimes do nothing in last m-cycle
    try {
        opcode->steps.at(current_m_cycles++)();
    }
    catch(const std::out_of_range& e) {}

    if (early_exit) {
        set_state(State::FETCH);
        return;
    }

    if (current_m_cycles == opcode_m_cycles) {

        /* Set master interrupt enable if scheduled - done here since EI effect
         * delays one opcode. */
        if (interrupt_enable_scheduled && opcode->name != "EI") {
            interrupt_manager->enable_interrupts();
            interrupt_enable_scheduled = false;
        }
        
        // Set next state
        if (interrupt_manager->interrupts_enabled() &&
            interrupt_manager->interrupt_requested()) {
            set_state(State::INTERRUPT);
        }
        else {
            set_state(State::FETCH);
            fetch_cycle(); // Fetch in same m-cycle as last working m-cycle
        }
    }
}

/* Carry out 1 m-cycle of the INTERRUPT state. 
 * - First cycle - wait.
 * - Second cycle - acknowledge interrupt.
 * - Third and fourth cycles - push reg.pc onto the stack.
 * - Final cycle - set reg.pc to corresponding handler address and fetch the
 * corresponding opcode. */
void Cpu::interrupt_cycle() {

    switch(current_m_cycles) {
        
        case 0:
            break;
        
        case 1:
            interrupt_type = interrupt_manager->get_enabled();
            if (interrupt_type == InterruptType::NONE) {
                set_state(State::FETCH);
                return;
            }
            interrupt_manager->acknowledge(interrupt_type);
            interrupt_manager->disable_interrupts();
            break;

        case 2:
            reg.sp--;
            mmu->write(reg.sp--, reg.pc >> 8);
            break;

        case 3:
            mmu->write(reg.sp, reg.pc & 0xFF);
            break;

        case 4:
            reg.pc = interrupt_manager->get_handler_address(interrupt_type);
            set_state(State::FETCH);
            fetch_cycle();
            return;
    }

    current_m_cycles++;
}

/* Switch state to INTERRUPT if interrupts are enabled and an interrupt is 
 * requested.
 * If an interrupt is requested but interrupts are not enabled then sets to 
 * FETCH (halt bug) and prepares to fetch and ignore the same HALT opcode. */
void Cpu::halt_cycle() {
    if (interrupt_manager->interrupt_requested()) {
        if (interrupt_manager->interrupts_enabled()) {
            set_state(State::INTERRUPT);
        }
        else {
            set_state(State::FETCH);
            reg.pc--;
            halt_bug = true;
        }
    }
}

// Return a string representation of the CPU.
std::string Cpu::representation() const {
    std::ostringstream repr;
    repr << "CPU:" << std::hex
        << " Opcode = " << static_cast<int>(opcode->address)
        << " A = " << static_cast<int>(reg.a)
        << " B = " << static_cast<int>(reg.b)
        << " C = " << static_cast<int>(reg.c)
        << " D = " << static_cast<int>(reg.d)
        << " E = " << static_cast<int>(reg.e)
        << " F = " << std::bitset<4>(reg.f() >> 4)
        << " H = " << static_cast<int>(reg.h)
        << " L = " << static_cast<int>(reg.l)
        << " SP = " << reg.sp
        << " PC = " << reg.pc;
    return repr.str();
}

// Initialise both opcode dictionaries.
void Cpu::initialise_opcodes_dictionaries() {
    
    opcodes[0x00] = opcode_0x00();
    opcodes[0x01] = opcode_0x01();
    opcodes[0x02] = opcode_0x02();
    opcodes[0x03] = opcode_0x03();
    opcodes[0x04] = opcode_0x04();
    opcodes[0x05] = opcode_0x05();
    opcodes[0x06] = opcode_0x06();
    opcodes[0x07] = opcode_0x07();
    opcodes[0x08] = opcode_0x08();
    opcodes[0x09] = opcode_0x09();
    opcodes[0x0a] = opcode_0x0a();
    opcodes[0x0b] = opcode_0x0b();
    opcodes[0x0c] = opcode_0x0c();
    opcodes[0x0d] = opcode_0x0d();
    opcodes[0x0e] = opcode_0x0e();
    opcodes[0x0f] = opcode_0x0f();
    opcodes[0x10] = opcode_0x10();
    opcodes[0x11] = opcode_0x11();
    opcodes[0x12] = opcode_0x12();
    opcodes[0x13] = opcode_0x13();
    opcodes[0x14] = opcode_0x14();
    opcodes[0x15] = opcode_0x15();
    opcodes[0x16] = opcode_0x16();
    opcodes[0x17] = opcode_0x17();
    opcodes[0x18] = opcode_0x18();
    opcodes[0x19] = opcode_0x19();
    opcodes[0x1a] = opcode_0x1a();
    opcodes[0x1b] = opcode_0x1b();
    opcodes[0x1c] = opcode_0x1c();
    opcodes[0x1d] = opcode_0x1d();
    opcodes[0x1e] = opcode_0x1e();
    opcodes[0x1f] = opcode_0x1f();
    opcodes[0x20] = opcode_0x20();
    opcodes[0x21] = opcode_0x21();
    opcodes[0x22] = opcode_0x22();
    opcodes[0x23] = opcode_0x23();
    opcodes[0x24] = opcode_0x24();
    opcodes[0x25] = opcode_0x25();
    opcodes[0x26] = opcode_0x26();
    opcodes[0x27] = opcode_0x27();
    opcodes[0x28] = opcode_0x28();
    opcodes[0x29] = opcode_0x29();
    opcodes[0x2a] = opcode_0x2a();
    opcodes[0x2b] = opcode_0x2b();
    opcodes[0x2c] = opcode_0x2c();
    opcodes[0x2d] = opcode_0x2d();
    opcodes[0x2e] = opcode_0x2e();
    opcodes[0x2f] = opcode_0x2f();
    opcodes[0x30] = opcode_0x30();
    opcodes[0x31] = opcode_0x31();
    opcodes[0x32] = opcode_0x32();
    opcodes[0x33] = opcode_0x33();
    opcodes[0x34] = opcode_0x34();
    opcodes[0x35] = opcode_0x35();
    opcodes[0x36] = opcode_0x36();
    opcodes[0x37] = opcode_0x37();
    opcodes[0x38] = opcode_0x38();
    opcodes[0x39] = opcode_0x39();
    opcodes[0x3a] = opcode_0x3a();
    opcodes[0x3b] = opcode_0x3b();
    opcodes[0x3c] = opcode_0x3c();
    opcodes[0x3d] = opcode_0x3d();
    opcodes[0x3e] = opcode_0x3e();
    opcodes[0x3f] = opcode_0x3f();
    opcodes[0x40] = opcode_0x40();
    opcodes[0x41] = opcode_0x41();
    opcodes[0x42] = opcode_0x42();
    opcodes[0x43] = opcode_0x43();
    opcodes[0x44] = opcode_0x44();
    opcodes[0x45] = opcode_0x45();
    opcodes[0x46] = opcode_0x46();
    opcodes[0x47] = opcode_0x47();
    opcodes[0x48] = opcode_0x48();
    opcodes[0x49] = opcode_0x49();
    opcodes[0x4a] = opcode_0x4a();
    opcodes[0x4b] = opcode_0x4b();
    opcodes[0x4c] = opcode_0x4c();
    opcodes[0x4d] = opcode_0x4d();
    opcodes[0x4e] = opcode_0x4e();
    opcodes[0x4f] = opcode_0x4f();
    opcodes[0x50] = opcode_0x50();
    opcodes[0x51] = opcode_0x51();
    opcodes[0x52] = opcode_0x52();
    opcodes[0x53] = opcode_0x53();
    opcodes[0x54] = opcode_0x54();
    opcodes[0x55] = opcode_0x55();
    opcodes[0x56] = opcode_0x56();
    opcodes[0x57] = opcode_0x57();
    opcodes[0x58] = opcode_0x58();
    opcodes[0x59] = opcode_0x59();
    opcodes[0x5a] = opcode_0x5a();
    opcodes[0x5b] = opcode_0x5b();
    opcodes[0x5c] = opcode_0x5c();
    opcodes[0x5d] = opcode_0x5d();
    opcodes[0x5e] = opcode_0x5e();
    opcodes[0x5f] = opcode_0x5f();
    opcodes[0x60] = opcode_0x60();
    opcodes[0x61] = opcode_0x61();
    opcodes[0x62] = opcode_0x62();
    opcodes[0x63] = opcode_0x63();
    opcodes[0x64] = opcode_0x64();
    opcodes[0x65] = opcode_0x65();
    opcodes[0x66] = opcode_0x66();
    opcodes[0x67] = opcode_0x67();
    opcodes[0x68] = opcode_0x68();
    opcodes[0x69] = opcode_0x69();
    opcodes[0x6a] = opcode_0x6a();
    opcodes[0x6b] = opcode_0x6b();
    opcodes[0x6c] = opcode_0x6c();
    opcodes[0x6d] = opcode_0x6d();
    opcodes[0x6e] = opcode_0x6e();
    opcodes[0x6f] = opcode_0x6f();
    opcodes[0x70] = opcode_0x70();
    opcodes[0x71] = opcode_0x71();
    opcodes[0x72] = opcode_0x72();
    opcodes[0x73] = opcode_0x73();
    opcodes[0x74] = opcode_0x74();
    opcodes[0x75] = opcode_0x75();
    opcodes[0x76] = opcode_0x76();
    opcodes[0x77] = opcode_0x77();
    opcodes[0x78] = opcode_0x78();
    opcodes[0x79] = opcode_0x79();
    opcodes[0x7a] = opcode_0x7a();
    opcodes[0x7b] = opcode_0x7b();
    opcodes[0x7c] = opcode_0x7c();
    opcodes[0x7d] = opcode_0x7d();
    opcodes[0x7e] = opcode_0x7e();
    opcodes[0x7f] = opcode_0x7f();
    opcodes[0x80] = opcode_0x80();
    opcodes[0x81] = opcode_0x81();
    opcodes[0x82] = opcode_0x82();
    opcodes[0x83] = opcode_0x83();
    opcodes[0x84] = opcode_0x84();
    opcodes[0x85] = opcode_0x85();
    opcodes[0x86] = opcode_0x86();
    opcodes[0x87] = opcode_0x87();
    opcodes[0x88] = opcode_0x88();
    opcodes[0x89] = opcode_0x89();
    opcodes[0x8a] = opcode_0x8a();
    opcodes[0x8b] = opcode_0x8b();
    opcodes[0x8c] = opcode_0x8c();
    opcodes[0x8d] = opcode_0x8d();
    opcodes[0x8e] = opcode_0x8e();
    opcodes[0x8f] = opcode_0x8f();
    opcodes[0x90] = opcode_0x90();
    opcodes[0x91] = opcode_0x91();
    opcodes[0x92] = opcode_0x92();
    opcodes[0x93] = opcode_0x93();
    opcodes[0x94] = opcode_0x94();
    opcodes[0x95] = opcode_0x95();
    opcodes[0x96] = opcode_0x96();
    opcodes[0x97] = opcode_0x97();
    opcodes[0x98] = opcode_0x98();
    opcodes[0x99] = opcode_0x99();
    opcodes[0x9a] = opcode_0x9a();
    opcodes[0x9b] = opcode_0x9b();
    opcodes[0x9c] = opcode_0x9c();
    opcodes[0x9d] = opcode_0x9d();
    opcodes[0x9e] = opcode_0x9e();
    opcodes[0x9f] = opcode_0x9f();
    opcodes[0xa0] = opcode_0xa0();
    opcodes[0xa1] = opcode_0xa1();
    opcodes[0xa2] = opcode_0xa2();
    opcodes[0xa3] = opcode_0xa3();
    opcodes[0xa4] = opcode_0xa4();
    opcodes[0xa5] = opcode_0xa5();
    opcodes[0xa6] = opcode_0xa6();
    opcodes[0xa7] = opcode_0xa7();
    opcodes[0xa8] = opcode_0xa8();
    opcodes[0xa9] = opcode_0xa9();
    opcodes[0xaa] = opcode_0xaa();
    opcodes[0xab] = opcode_0xab();
    opcodes[0xac] = opcode_0xac();
    opcodes[0xad] = opcode_0xad();
    opcodes[0xae] = opcode_0xae();
    opcodes[0xaf] = opcode_0xaf();
    opcodes[0xb0] = opcode_0xb0();
    opcodes[0xb1] = opcode_0xb1();
    opcodes[0xb2] = opcode_0xb2();
    opcodes[0xb3] = opcode_0xb3();
    opcodes[0xb4] = opcode_0xb4();
    opcodes[0xb5] = opcode_0xb5();
    opcodes[0xb6] = opcode_0xb6();
    opcodes[0xb7] = opcode_0xb7();
    opcodes[0xb8] = opcode_0xb8();
    opcodes[0xb9] = opcode_0xb9();
    opcodes[0xba] = opcode_0xba();
    opcodes[0xbb] = opcode_0xbb();
    opcodes[0xbc] = opcode_0xbc();
    opcodes[0xbd] = opcode_0xbd();
    opcodes[0xbe] = opcode_0xbe();
    opcodes[0xbf] = opcode_0xbf();
    opcodes[0xc0] = opcode_0xc0();
    opcodes[0xc1] = opcode_0xc1();
    opcodes[0xc2] = opcode_0xc2();
    opcodes[0xc3] = opcode_0xc3();
    opcodes[0xc4] = opcode_0xc4();
    opcodes[0xc5] = opcode_0xc5();
    opcodes[0xc6] = opcode_0xc6();
    opcodes[0xc7] = opcode_0xc7();
    opcodes[0xc8] = opcode_0xc8();
    opcodes[0xc9] = opcode_0xc9();
    opcodes[0xca] = opcode_0xca();
    opcodes[0xcb] = opcode_0xcb();
    opcodes[0xcc] = opcode_0xcc();
    opcodes[0xcd] = opcode_0xcd();
    opcodes[0xce] = opcode_0xce();
    opcodes[0xcf] = opcode_0xcf();
    opcodes[0xd0] = opcode_0xd0();
    opcodes[0xd1] = opcode_0xd1();
    opcodes[0xd2] = opcode_0xd2();
    opcodes[0xd4] = opcode_0xd4();
    opcodes[0xd5] = opcode_0xd5();
    opcodes[0xd6] = opcode_0xd6();
    opcodes[0xd7] = opcode_0xd7();
    opcodes[0xd8] = opcode_0xd8();
    opcodes[0xd9] = opcode_0xd9();
    opcodes[0xda] = opcode_0xda();
    opcodes[0xdc] = opcode_0xdc();
    opcodes[0xde] = opcode_0xde();
    opcodes[0xdf] = opcode_0xdf();
    opcodes[0xe0] = opcode_0xe0();
    opcodes[0xe1] = opcode_0xe1();
    opcodes[0xe2] = opcode_0xe2();
    opcodes[0xe5] = opcode_0xe5();
    opcodes[0xe6] = opcode_0xe6();
    opcodes[0xe7] = opcode_0xe7();
    opcodes[0xe8] = opcode_0xe8();
    opcodes[0xe9] = opcode_0xe9();
    opcodes[0xea] = opcode_0xea();
    opcodes[0xee] = opcode_0xee();
    opcodes[0xef] = opcode_0xef();
    opcodes[0xf0] = opcode_0xf0();
    opcodes[0xf1] = opcode_0xf1();
    opcodes[0xf2] = opcode_0xf2();
    opcodes[0xf3] = opcode_0xf3();
    opcodes[0xf5] = opcode_0xf5();
    opcodes[0xf6] = opcode_0xf6();
    opcodes[0xf7] = opcode_0xf7();
    opcodes[0xf8] = opcode_0xf8();
    opcodes[0xf9] = opcode_0xf9();
    opcodes[0xfa] = opcode_0xfa();
    opcodes[0xfb] = opcode_0xfb();
    opcodes[0xfe] = opcode_0xfe();
    opcodes[0xff] = opcode_0xff();

    cb_opcodes[0x00] = opcode_cb_0x00();
    cb_opcodes[0x01] = opcode_cb_0x01();
    cb_opcodes[0x02] = opcode_cb_0x02();
    cb_opcodes[0x03] = opcode_cb_0x03();
    cb_opcodes[0x04] = opcode_cb_0x04();
    cb_opcodes[0x05] = opcode_cb_0x05();
    cb_opcodes[0x06] = opcode_cb_0x06();
    cb_opcodes[0x07] = opcode_cb_0x07();
    cb_opcodes[0x08] = opcode_cb_0x08();
    cb_opcodes[0x09] = opcode_cb_0x09();
    cb_opcodes[0x0a] = opcode_cb_0x0a();
    cb_opcodes[0x0b] = opcode_cb_0x0b();
    cb_opcodes[0x0c] = opcode_cb_0x0c();
    cb_opcodes[0x0d] = opcode_cb_0x0d();
    cb_opcodes[0x0e] = opcode_cb_0x0e();
    cb_opcodes[0x0f] = opcode_cb_0x0f();
    cb_opcodes[0x10] = opcode_cb_0x10();
    cb_opcodes[0x11] = opcode_cb_0x11();
    cb_opcodes[0x12] = opcode_cb_0x12();
    cb_opcodes[0x13] = opcode_cb_0x13();
    cb_opcodes[0x14] = opcode_cb_0x14();
    cb_opcodes[0x15] = opcode_cb_0x15();
    cb_opcodes[0x16] = opcode_cb_0x16();
    cb_opcodes[0x17] = opcode_cb_0x17();
    cb_opcodes[0x18] = opcode_cb_0x18();
    cb_opcodes[0x19] = opcode_cb_0x19();
    cb_opcodes[0x1a] = opcode_cb_0x1a();
    cb_opcodes[0x1b] = opcode_cb_0x1b();
    cb_opcodes[0x1c] = opcode_cb_0x1c();
    cb_opcodes[0x1d] = opcode_cb_0x1d();
    cb_opcodes[0x1e] = opcode_cb_0x1e();
    cb_opcodes[0x1f] = opcode_cb_0x1f();
    cb_opcodes[0x20] = opcode_cb_0x20();
    cb_opcodes[0x21] = opcode_cb_0x21();
    cb_opcodes[0x22] = opcode_cb_0x22();
    cb_opcodes[0x23] = opcode_cb_0x23();
    cb_opcodes[0x24] = opcode_cb_0x24();
    cb_opcodes[0x25] = opcode_cb_0x25();
    cb_opcodes[0x26] = opcode_cb_0x26();
    cb_opcodes[0x27] = opcode_cb_0x27();
    cb_opcodes[0x28] = opcode_cb_0x28();
    cb_opcodes[0x29] = opcode_cb_0x29();
    cb_opcodes[0x2a] = opcode_cb_0x2a();
    cb_opcodes[0x2b] = opcode_cb_0x2b();
    cb_opcodes[0x2c] = opcode_cb_0x2c();
    cb_opcodes[0x2d] = opcode_cb_0x2d();
    cb_opcodes[0x2e] = opcode_cb_0x2e();
    cb_opcodes[0x2f] = opcode_cb_0x2f();
    cb_opcodes[0x30] = opcode_cb_0x30();
    cb_opcodes[0x31] = opcode_cb_0x31();
    cb_opcodes[0x32] = opcode_cb_0x32();
    cb_opcodes[0x33] = opcode_cb_0x33();
    cb_opcodes[0x34] = opcode_cb_0x34();
    cb_opcodes[0x35] = opcode_cb_0x35();
    cb_opcodes[0x36] = opcode_cb_0x36();
    cb_opcodes[0x37] = opcode_cb_0x37();
    cb_opcodes[0x38] = opcode_cb_0x38();
    cb_opcodes[0x39] = opcode_cb_0x39();
    cb_opcodes[0x3a] = opcode_cb_0x3a();
    cb_opcodes[0x3b] = opcode_cb_0x3b();
    cb_opcodes[0x3c] = opcode_cb_0x3c();
    cb_opcodes[0x3d] = opcode_cb_0x3d();
    cb_opcodes[0x3e] = opcode_cb_0x3e();
    cb_opcodes[0x3f] = opcode_cb_0x3f();
    cb_opcodes[0x40] = opcode_cb_0x40();
    cb_opcodes[0x41] = opcode_cb_0x41();
    cb_opcodes[0x42] = opcode_cb_0x42();
    cb_opcodes[0x43] = opcode_cb_0x43();
    cb_opcodes[0x44] = opcode_cb_0x44();
    cb_opcodes[0x45] = opcode_cb_0x45();
    cb_opcodes[0x46] = opcode_cb_0x46();
    cb_opcodes[0x47] = opcode_cb_0x47();
    cb_opcodes[0x48] = opcode_cb_0x48();
    cb_opcodes[0x49] = opcode_cb_0x49();
    cb_opcodes[0x4a] = opcode_cb_0x4a();
    cb_opcodes[0x4b] = opcode_cb_0x4b();
    cb_opcodes[0x4c] = opcode_cb_0x4c();
    cb_opcodes[0x4d] = opcode_cb_0x4d();
    cb_opcodes[0x4e] = opcode_cb_0x4e();
    cb_opcodes[0x4f] = opcode_cb_0x4f();
    cb_opcodes[0x50] = opcode_cb_0x50();
    cb_opcodes[0x51] = opcode_cb_0x51();
    cb_opcodes[0x52] = opcode_cb_0x52();
    cb_opcodes[0x53] = opcode_cb_0x53();
    cb_opcodes[0x54] = opcode_cb_0x54();
    cb_opcodes[0x55] = opcode_cb_0x55();
    cb_opcodes[0x56] = opcode_cb_0x56();
    cb_opcodes[0x57] = opcode_cb_0x57();
    cb_opcodes[0x58] = opcode_cb_0x58();
    cb_opcodes[0x59] = opcode_cb_0x59();
    cb_opcodes[0x5a] = opcode_cb_0x5a();
    cb_opcodes[0x5b] = opcode_cb_0x5b();
    cb_opcodes[0x5c] = opcode_cb_0x5c();
    cb_opcodes[0x5d] = opcode_cb_0x5d();
    cb_opcodes[0x5e] = opcode_cb_0x5e();
    cb_opcodes[0x5f] = opcode_cb_0x5f();
    cb_opcodes[0x60] = opcode_cb_0x60();
    cb_opcodes[0x61] = opcode_cb_0x61();
    cb_opcodes[0x62] = opcode_cb_0x62();
    cb_opcodes[0x63] = opcode_cb_0x63();
    cb_opcodes[0x64] = opcode_cb_0x64();
    cb_opcodes[0x65] = opcode_cb_0x65();
    cb_opcodes[0x66] = opcode_cb_0x66();
    cb_opcodes[0x67] = opcode_cb_0x67();
    cb_opcodes[0x68] = opcode_cb_0x68();
    cb_opcodes[0x69] = opcode_cb_0x69();
    cb_opcodes[0x6a] = opcode_cb_0x6a();
    cb_opcodes[0x6b] = opcode_cb_0x6b();
    cb_opcodes[0x6c] = opcode_cb_0x6c();
    cb_opcodes[0x6d] = opcode_cb_0x6d();
    cb_opcodes[0x6e] = opcode_cb_0x6e();
    cb_opcodes[0x6f] = opcode_cb_0x6f();
    cb_opcodes[0x70] = opcode_cb_0x70();
    cb_opcodes[0x71] = opcode_cb_0x71();
    cb_opcodes[0x72] = opcode_cb_0x72();
    cb_opcodes[0x73] = opcode_cb_0x73();
    cb_opcodes[0x74] = opcode_cb_0x74();
    cb_opcodes[0x75] = opcode_cb_0x75();
    cb_opcodes[0x76] = opcode_cb_0x76();
    cb_opcodes[0x77] = opcode_cb_0x77();
    cb_opcodes[0x78] = opcode_cb_0x78();
    cb_opcodes[0x79] = opcode_cb_0x79();
    cb_opcodes[0x7a] = opcode_cb_0x7a();
    cb_opcodes[0x7b] = opcode_cb_0x7b();
    cb_opcodes[0x7c] = opcode_cb_0x7c();
    cb_opcodes[0x7d] = opcode_cb_0x7d();
    cb_opcodes[0x7e] = opcode_cb_0x7e();
    cb_opcodes[0x7f] = opcode_cb_0x7f();
    cb_opcodes[0x80] = opcode_cb_0x80();
    cb_opcodes[0x81] = opcode_cb_0x81();
    cb_opcodes[0x82] = opcode_cb_0x82();
    cb_opcodes[0x83] = opcode_cb_0x83();
    cb_opcodes[0x84] = opcode_cb_0x84();
    cb_opcodes[0x85] = opcode_cb_0x85();
    cb_opcodes[0x86] = opcode_cb_0x86();
    cb_opcodes[0x87] = opcode_cb_0x87();
    cb_opcodes[0x88] = opcode_cb_0x88();
    cb_opcodes[0x89] = opcode_cb_0x89();
    cb_opcodes[0x8a] = opcode_cb_0x8a();
    cb_opcodes[0x8b] = opcode_cb_0x8b();
    cb_opcodes[0x8c] = opcode_cb_0x8c();
    cb_opcodes[0x8d] = opcode_cb_0x8d();
    cb_opcodes[0x8e] = opcode_cb_0x8e();
    cb_opcodes[0x8f] = opcode_cb_0x8f();
    cb_opcodes[0x90] = opcode_cb_0x90();
    cb_opcodes[0x91] = opcode_cb_0x91();
    cb_opcodes[0x92] = opcode_cb_0x92();
    cb_opcodes[0x93] = opcode_cb_0x93();
    cb_opcodes[0x94] = opcode_cb_0x94();
    cb_opcodes[0x95] = opcode_cb_0x95();
    cb_opcodes[0x96] = opcode_cb_0x96();
    cb_opcodes[0x97] = opcode_cb_0x97();
    cb_opcodes[0x98] = opcode_cb_0x98();
    cb_opcodes[0x99] = opcode_cb_0x99();
    cb_opcodes[0x9a] = opcode_cb_0x9a();
    cb_opcodes[0x9b] = opcode_cb_0x9b();
    cb_opcodes[0x9c] = opcode_cb_0x9c();
    cb_opcodes[0x9d] = opcode_cb_0x9d();
    cb_opcodes[0x9e] = opcode_cb_0x9e();
    cb_opcodes[0x9f] = opcode_cb_0x9f();
    cb_opcodes[0xa0] = opcode_cb_0xa0();
    cb_opcodes[0xa1] = opcode_cb_0xa1();
    cb_opcodes[0xa2] = opcode_cb_0xa2();
    cb_opcodes[0xa3] = opcode_cb_0xa3();
    cb_opcodes[0xa4] = opcode_cb_0xa4();
    cb_opcodes[0xa5] = opcode_cb_0xa5();
    cb_opcodes[0xa6] = opcode_cb_0xa6();
    cb_opcodes[0xa7] = opcode_cb_0xa7();
    cb_opcodes[0xa8] = opcode_cb_0xa8();
    cb_opcodes[0xa9] = opcode_cb_0xa9();
    cb_opcodes[0xaa] = opcode_cb_0xaa();
    cb_opcodes[0xab] = opcode_cb_0xab();
    cb_opcodes[0xac] = opcode_cb_0xac();
    cb_opcodes[0xad] = opcode_cb_0xad();
    cb_opcodes[0xae] = opcode_cb_0xae();
    cb_opcodes[0xaf] = opcode_cb_0xaf();
    cb_opcodes[0xb0] = opcode_cb_0xb0();
    cb_opcodes[0xb1] = opcode_cb_0xb1();
    cb_opcodes[0xb2] = opcode_cb_0xb2();
    cb_opcodes[0xb3] = opcode_cb_0xb3();
    cb_opcodes[0xb4] = opcode_cb_0xb4();
    cb_opcodes[0xb5] = opcode_cb_0xb5();
    cb_opcodes[0xb6] = opcode_cb_0xb6();
    cb_opcodes[0xb7] = opcode_cb_0xb7();
    cb_opcodes[0xb8] = opcode_cb_0xb8();
    cb_opcodes[0xb9] = opcode_cb_0xb9();
    cb_opcodes[0xba] = opcode_cb_0xba();
    cb_opcodes[0xbb] = opcode_cb_0xbb();
    cb_opcodes[0xbc] = opcode_cb_0xbc();
    cb_opcodes[0xbd] = opcode_cb_0xbd();
    cb_opcodes[0xbe] = opcode_cb_0xbe();
    cb_opcodes[0xbf] = opcode_cb_0xbf();
    cb_opcodes[0xc0] = opcode_cb_0xc0();
    cb_opcodes[0xc1] = opcode_cb_0xc1();
    cb_opcodes[0xc2] = opcode_cb_0xc2();
    cb_opcodes[0xc3] = opcode_cb_0xc3();
    cb_opcodes[0xc4] = opcode_cb_0xc4();
    cb_opcodes[0xc5] = opcode_cb_0xc5();
    cb_opcodes[0xc6] = opcode_cb_0xc6();
    cb_opcodes[0xc7] = opcode_cb_0xc7();
    cb_opcodes[0xc8] = opcode_cb_0xc8();
    cb_opcodes[0xc9] = opcode_cb_0xc9();
    cb_opcodes[0xca] = opcode_cb_0xca();
    cb_opcodes[0xcb] = opcode_cb_0xcb();
    cb_opcodes[0xcc] = opcode_cb_0xcc();
    cb_opcodes[0xcd] = opcode_cb_0xcd();
    cb_opcodes[0xce] = opcode_cb_0xce();
    cb_opcodes[0xcf] = opcode_cb_0xcf();
    cb_opcodes[0xd0] = opcode_cb_0xd0();
    cb_opcodes[0xd1] = opcode_cb_0xd1();
    cb_opcodes[0xd2] = opcode_cb_0xd2();
    cb_opcodes[0xd3] = opcode_cb_0xd3();
    cb_opcodes[0xd4] = opcode_cb_0xd4();
    cb_opcodes[0xd5] = opcode_cb_0xd5();
    cb_opcodes[0xd6] = opcode_cb_0xd6();
    cb_opcodes[0xd7] = opcode_cb_0xd7();
    cb_opcodes[0xd8] = opcode_cb_0xd8();
    cb_opcodes[0xd9] = opcode_cb_0xd9();
    cb_opcodes[0xda] = opcode_cb_0xda();
    cb_opcodes[0xdb] = opcode_cb_0xdb();
    cb_opcodes[0xdc] = opcode_cb_0xdc();
    cb_opcodes[0xdd] = opcode_cb_0xdd();
    cb_opcodes[0xde] = opcode_cb_0xde();
    cb_opcodes[0xdf] = opcode_cb_0xdf();
    cb_opcodes[0xe0] = opcode_cb_0xe0();
    cb_opcodes[0xe1] = opcode_cb_0xe1();
    cb_opcodes[0xe2] = opcode_cb_0xe2();
    cb_opcodes[0xe3] = opcode_cb_0xe3();
    cb_opcodes[0xe4] = opcode_cb_0xe4();
    cb_opcodes[0xe5] = opcode_cb_0xe5();
    cb_opcodes[0xe6] = opcode_cb_0xe6();
    cb_opcodes[0xe7] = opcode_cb_0xe7();
    cb_opcodes[0xe8] = opcode_cb_0xe8();
    cb_opcodes[0xe9] = opcode_cb_0xe9();
    cb_opcodes[0xea] = opcode_cb_0xea();
    cb_opcodes[0xeb] = opcode_cb_0xeb();
    cb_opcodes[0xec] = opcode_cb_0xec();
    cb_opcodes[0xed] = opcode_cb_0xed();
    cb_opcodes[0xee] = opcode_cb_0xee();
    cb_opcodes[0xef] = opcode_cb_0xef();
    cb_opcodes[0xf0] = opcode_cb_0xf0();
    cb_opcodes[0xf1] = opcode_cb_0xf1();
    cb_opcodes[0xf2] = opcode_cb_0xf2();
    cb_opcodes[0xf3] = opcode_cb_0xf3();
    cb_opcodes[0xf4] = opcode_cb_0xf4();
    cb_opcodes[0xf5] = opcode_cb_0xf5();
    cb_opcodes[0xf6] = opcode_cb_0xf6();
    cb_opcodes[0xf7] = opcode_cb_0xf7();
    cb_opcodes[0xf8] = opcode_cb_0xf8();
    cb_opcodes[0xf9] = opcode_cb_0xf9();
    cb_opcodes[0xfa] = opcode_cb_0xfa();
    cb_opcodes[0xfb] = opcode_cb_0xfb();
    cb_opcodes[0xfc] = opcode_cb_0xfc();
    cb_opcodes[0xfd] = opcode_cb_0xfd();
    cb_opcodes[0xfe] = opcode_cb_0xfe();
    cb_opcodes[0xff] = opcode_cb_0xff();
}