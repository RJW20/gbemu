#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "mbc/base_mbc.hpp"

// Simulates a physical cartridge with ROM and (external) RAM.
class Cartridge {
public:
    Cartridge(std::string rom_path);
    ~Cartridge() {};

    std::string title;  // ROM title

    uint8_t read_rom(uint16_t address) const;
    void write_rom(uint16_t address, uint8_t value);
    uint8_t read_ram(uint16_t address) const;
    void write_ram(uint16_t address, uint8_t value);

private:
    std::unique_ptr<BaseMbc> mbc;   // Memory bank controller

    // Cartridge header constants
    static constexpr uint16_t TITLE_START_ADDRESS = 0x134;
    static constexpr uint8_t TITLE_LENGTH = 0x10;
    static constexpr uint16_t MBC_CODE_ADDRESS = 0x147;
};

#endif