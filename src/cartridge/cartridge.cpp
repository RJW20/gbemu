#include <iostream>
#include <string>
#include <fstream>
#include "cartridge.hpp"

// Load the ROM into rom.
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
    length = (int) size;
    file.seekg(0, std::ios::beg);
    rom.resize(size);

    // Read the file into the rom vector
    if (!file.read(reinterpret_cast<char*>(rom.data()), size)) {
        std::cerr << "Failed to read ROM: " << rom_path << std::endl;
        exit(0);
    }
    file.close();
}

// Read from the rom.
uint8_t Cartridge::read(uint16_t address) const {

    if (address >= length) {
        std::cerr << "Invalid cartridge read at address: " << std::hex <<
            address << std::endl;
        return 0xFF;
    }

    return rom[address];
}

/* Write to the rom.
 * Interpreted as an opcode by the MBC. */
void Cartridge::write(uint16_t address, uint8_t value) {
    std::cerr << "ROM \'writes\' currently not implemented." << std::endl;
}