#include <iostream>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <vector>
#include "base_mbc.hpp"
#include "lookup.hpp"

/* Load the given rom_file into the rom vector and read the cartridge header.
 * Initialises the external ram if required.
 * Does not take ownership of the given std::ifstream. */
BaseMbc::BaseMbc(std::ifstream& rom_file, bool has_external_ram) {

    // Read the rom into 16 KB banks
    rom_file.seekg(0, std::ios::beg);
    while (!rom_file.eof()) {
        std::vector<uint8_t> bank(ROM_BANK_SIZE, 0);
        rom_file.read(reinterpret_cast<char*>(bank.data()), ROM_BANK_SIZE);
        std::size_t bytes_read = rom_file.gcount();
        if (bytes_read > 0) {
            bank.resize(bytes_read);
            rom.push_back(std::move(bank));
        }
    }

    // ROM bank handling
    rom_size = rom[0][ROM_SIZE_ADDRESS];
    if (rom.size() != (1 << (rom_size + 1))) {
        std::cerr << "Cartridge header has incorrect ROM size value at "
            << "address: " << std::hex << (uint16_t) ROM_SIZE_ADDRESS
            << std::endl;
        exit(20);
    }

    // RAM bank handling
    try {
        ram_size = has_external_ram ?
            external_ram_lookup.at(rom[0][RAM_SIZE_ADDRESS]) : 0;
        reset();
    }
    catch(const std::out_of_range& ex) {
        std::cerr << "Cartridge header has invalid RAM size value at "
            << "address: " << std::hex << (uint16_t) RAM_SIZE_ADDRESS
            << std::endl;
        exit(21);
    }
}

// Clear RAM.
void BaseMbc::reset() {
    ram.resize(ram_size, std::vector<uint8_t>(RAM_BANK_SIZE));
}