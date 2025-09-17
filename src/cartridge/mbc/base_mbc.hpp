#ifndef BASE_MBC_HPP
#define BASE_MBC_HPP

#include <cstdint>
#include <fstream>
#include <vector>
#include <array>

// Base class for a Memory Bank Controller.
class BaseMbc {
public:
    BaseMbc(std::ifstream& rom_file, bool has_external_ram);
    virtual ~BaseMbc() = default;

    virtual void reset();

    virtual uint8_t read_rom(uint16_t address) const = 0;
    virtual void write_rom(uint16_t address, uint8_t value) = 0;
    virtual uint8_t read_ram(uint16_t address) const = 0;
    virtual void write_ram(uint16_t address, uint8_t value) = 0;

protected:
    static constexpr uint16_t ROM_BANK_SIZE = 0x4000;   // 16 KB
    static constexpr uint16_t RAM_BANK_SIZE = 0x2000;   // 8 KB

    std::vector<std::array<uint8_t, ROM_BANK_SIZE>> rom;  // ROM banks
    std::vector<std::array<uint8_t, RAM_BANK_SIZE>> ram;  // RAM banks

    // Cartridge header values
    int rom_size;       // Total number of ROM banks
    int ram_size;       // Total number of RAM banks

private:
    // Cartridge header constants
    static constexpr uint16_t ROM_SIZE_ADDRESS = 0x148;
    static constexpr uint16_t RAM_SIZE_ADDRESS = 0x149;
};

#endif