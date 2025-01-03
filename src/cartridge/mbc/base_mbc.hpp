#ifndef BASE_MBC_HPP
#define BASE_MBC_HPP

#include <cstdint>
#include <cstddef>
#include <fstream>

// Base class for a Memory Bank Controller.
class BaseMbc {
public:
    BaseMbc(std::ifstream& rom_file) {};
    virtual ~BaseMbc() {};

    virtual uint8_t read_rom(uint16_t address) const = 0;
    virtual void write_rom(uint16_t address, uint8_t value) = 0;
    virtual uint8_t read_ram(uint16_t address) const = 0;
    virtual void write_ram(uint16_t address, uint8_t value) = 0;

protected:
    std::vector<std::vector<uint8_t>> rom;  // Vector of ROM banks
    std::vector<std::vector<uint8_t>> ram;  // Vector of RAM banks

    int rom_banks;          // Total number of ROM banks
    int selected_rom_bank;  // Currently selected ROM bank
    bool ram_banks_enabled; // Flag determining if cartridge has external RAM
    int ram_banks;          // Total number of RAM banks
    int selected_ram_bank;  // Currently selected RAM bank
    //bool battery;           // Flag determining if cartridge has a battery

private:
    static constexpr size_t ROM_BANK_SIZE = 0x4000;   // 16 KB
    static constexpr size_t RAM_BANK_SIZE = 0x2000;   // 8 KB

    // Cartridge header constants
    static constexpr uint8_t ROM_SIZE_ADDRESS = 0x148;
    static constexpr uint8_t RAM_SIZE_ADDRESS = 0x149;
};

#endif