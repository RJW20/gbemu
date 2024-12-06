#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>
#include <vector>

class Mmu {
public:
    Mmu();
    ~Mmu() {};

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

private:
};

#endif