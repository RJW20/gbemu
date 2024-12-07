#include <iostream>
#include <string>
#include <fstream>
#include "cartridge.hpp"

// Load the ROM into rom and initialise the ram.
Cartridge::Cartridge(std::string rom_path) {
    
    std::cout << "Loading ROM: " << rom_path << std::endl;

    // Open the file in binary mode
    std::ifstream file(rom_path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open ROM: " << rom_path << std::endl;
        exit(0);
    }

    // Get the file size
    std::streamsize size = file.tellg();
    rom_size = (int) size;
    file.seekg(0, std::ios::beg);
    rom.resize(size);

    // Read the file into the rom vector
    if (!file.read(reinterpret_cast<char*>(rom.data()), size)) {
        std::cerr << "Failed to read ROM: " << rom_path << std::endl;
        exit(0);
    }
    file.close();

    // Initialise the ram
    ram_size = 0x2000;  // 8 KB
    ram.resize(ram_size);
}

// Return the 8 bit value in the rom at the given address.
uint8_t Cartridge::read_rom(uint16_t address) const {

    if (address >= rom_size) {
        std::cerr << "Invalid cartridge ROM read at address: " << std::hex <<
            address << std::endl;
        return 0xFF;
    }

    return rom[address];
}

/* Write the given 8 bit value to the rom at the given address.
 * Interpreted as an opcode by the MBC. */
void Cartridge::write_rom(uint16_t address, uint8_t value) {
    std::cerr << "ROM \'writes\' currently not implemented." << std::endl;
}

// Return the 8 bit value in the ram at the given address.
uint8_t Cartridge::read_ram(uint16_t address) const {

    if (address >= ram_size) {
        std::cerr << "Invalid cartridge RAM read at address: " << std::hex <<
            address << std::endl;
        return 0xFF;
    }

    return ram[address];
}

// Write the given 8 bit value to the ram at the given address.
void Cartridge::write_ram(uint16_t address, uint8_t value) {

    if (address >= ram_size) {
        std::cerr << "Invalid cartridge RAM write at address: " << std::hex <<
            address << std::endl;
        return;
    }

    ram[address] = value;

}