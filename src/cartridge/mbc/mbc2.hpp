#ifndef MBC2_HPP
#define MBC2_HPP

#include <cstdint>
#include <fstream>
#include "base_mbc.hpp"

// Memory Bank Controller version 2.
class Mbc2 : public BaseMbc {
public:
    Mbc2(std::ifstream& rom_file);
    ~Mbc2() override {};

    void reset() override;

    uint8_t read_rom(uint16_t address) const override;
    void write_rom(uint16_t address, uint8_t value) override;
    uint8_t read_ram(uint16_t address) const override;
    void write_ram(uint16_t address, uint8_t value) override;

private:
    // Registers
    bool is_ram_enabled;        // Controls if RAM is accessible
    uint8_t rom_bank_number;    // Currently selected ROM bank (4 bit)

    // Write regions
    static constexpr uint16_t RAM_BANK_LOWER = 0xA000;
};

#endif