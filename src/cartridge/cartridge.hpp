#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <string>
#include <vector>
#include <cstdint>

class Cartridge {
public:
    Cartridge(std::string rom_path);
    ~Cartridge() {};

    uint8_t read_rom(uint16_t address) const;
    void write_rom(uint16_t address, uint8_t value);

    uint8_t read_ram(uint16_t address) const;
    void write_ram(uint16_t address, uint8_t value);

private:
    int rom_size;               // Size of the ROM
    std::vector<uint8_t> rom;   // Vector to store ROM in
    int ram_size;               // Size of the RAM
    std::vector<uint8_t> ram;   // Vector to store RAM in
};

#endif