#ifndef MBC1_HPP
#define MBC1_HPP

#include <cstdint>
#include <fstream>
#include "base_mbc.hpp"

// Memory Bank Controller version 1.
class Mbc1 : public BaseMbc {
public:
    Mbc1(std::ifstream& rom_file, bool has_external_ram) : 
        BaseMbc(rom_file, has_external_ram) { reset(); }

    void reset() override;

    uint8_t read_rom(uint16_t address) const override;
    void write_rom(uint16_t address, uint8_t value) override;
    uint8_t read_ram(uint16_t address) const override;
    void write_ram(uint16_t address, uint8_t value) override;

private:
    // Registers
    bool is_ram_enabled;        // Controls if RAM is accessible
    uint8_t rom_bank_number;    // Currently selected ROM bank (5 bit)
    uint8_t ram_bank_number;    // Currently selected RAM bank (2 bit)
    bool advanced_banking;      // Controls behaviour of ram_bank_number

    // Write regions
    static constexpr uint16_t IS_RAM_ENABLED_UPPER = 0x2000;
    static constexpr uint16_t RAM_BANK_NUMBER_UPPER = 0x6000;
};

#endif