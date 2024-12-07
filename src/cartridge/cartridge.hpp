#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <string>
#include <vector>
#include <cstdint>

class Cartridge {
public:
    Cartridge(std::string rom_path);
    ~Cartridge() {};

    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);

private:
    int length;                 // Length of the ROM
    std::vector<uint8_t> rom;   // Vector to store ROM in
};

#endif