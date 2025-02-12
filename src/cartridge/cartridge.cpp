#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>
#include "../logger.hpp"
#include "cartridge.hpp"
#include "mbc/lookup.hpp"

// Load the ROM at the given path and pass it to the required MBC.
Cartridge::Cartridge(const std::string& rom_path) {
    
    Log::info("Loading ROM: " + rom_path);

    // Open the file in binary mode
    std::ifstream rom_file(rom_path, std::ios::binary | std::ios::ate);
    if (!rom_file) {
        Log::error("Failed to open ROM: " + rom_path);
        exit(0);
    }

    // Set the ROM title
    rom_file.seekg(TITLE_START_ADDRESS, std::ios::beg);
    char title_buffer[TITLE_LENGTH] = {};
    rom_file.read(title_buffer, TITLE_LENGTH);
    title = std::string(title_buffer, TITLE_LENGTH);

    // Instantiate the correct MBC
    rom_file.seekg(MBC_CODE_ADDRESS, std::ios::beg);
    uint8_t mbc_code = rom_file.get();
    try {
        mbc = mbc_lookup.at(mbc_code)(rom_file);
    }
    catch(const std::out_of_range& e) {
        std::ostringstream code;
        code << std::hex << static_cast<int>(mbc_code);
        Log::error("Unimplemented ROM type with MBC code: " + code.str());
        exit(1);
    }
}

// Reset the MBC.
void Cartridge::reset() {
    mbc->reset();
}

// Return the 8 bit value the MBC reads from its ROM at the given address.
uint8_t Cartridge::read_rom(uint16_t address) const {
    return mbc->read_rom(address);
}

/* Write the given 8 bit value to the MBC's ROM at the given address.
 * Interpreted as an opcode by the MBC. */
void Cartridge::write_rom(uint16_t address, uint8_t value) {
    mbc->write_rom(address, value);
}

// Return the 8 bit value the MBC reads from its RAM at the given address.
uint8_t Cartridge::read_ram(uint16_t address) const {
    return mbc->read_ram(address);
}

// Write the given 8 bit value to the MBC's RAM at the given address.
void Cartridge::write_ram(uint16_t address, uint8_t value) {
    mbc->write_ram(address, value);
}