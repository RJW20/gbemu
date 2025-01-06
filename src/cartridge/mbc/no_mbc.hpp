#ifndef NO_MBC_HPP
#define NO_MBC_HPP

#include <cstdint>
#include <fstream>
#include "base_mbc.hpp"

/* A class for cartridges with no Memory Bank Controller.
 * Used for small games with up to 32KB ROM.
 * Directly maps the ROM to 0x0000 - 0x7FFF. */
class NoMbc : public BaseMbc {
public:
    NoMbc(std::ifstream& rom_file) : BaseMbc(rom_file, false) {};
    ~NoMbc() override {};

    virtual void reset();

    uint8_t read_rom(uint16_t address) const override;
    void write_rom(uint16_t address, uint8_t value) override;
    uint8_t read_ram(uint16_t address) const override;
    void write_ram(uint16_t address, uint8_t value) override;
};

#endif