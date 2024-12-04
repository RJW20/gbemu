#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>
#include <vector>

class Memory {
public:

    Memory();
    ~Memory() {};

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

private:
    std::vector<uint8_t> memory;
};

#endif