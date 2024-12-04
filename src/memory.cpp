#include <iostream>
#include <cstdint>
#include "memory.hpp"

// Initialise the memory vector to the correct size.
Memory::Memory() {
    memory.resize(0xFFFF, 0);
}

// Return the byte in memory at the given address.
uint8_t Memory::read(uint16_t address) {
    return memory[address];
}

// Write the given byte to memory at the given address.
void Memory::write(uint16_t address, uint8_t value) {
    // TODO: don't allow writes below 0x8000
    memory[address] = value;
}