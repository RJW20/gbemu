#include <cstdint>
#include <cstddef>
#include <fstream>
#include <array>
#include <sstream>
#include "../../logger.hpp"
#include "base_mbc.hpp"
#include "lookup.hpp"

/* Load the given rom_file into the ROM banks and read the cartridge header.
 * Initialises the external ram if required.
 * Does not take ownership of the given std::ifstream. */
BaseMbc::BaseMbc(std::ifstream& rom_file, bool has_external_ram) {

    // Read the rom into 16 KB banks
    rom_file.seekg(0, std::ios::beg);
    while (!rom_file.eof()) {
        std::array<uint8_t, ROM_BANK_SIZE> bank;
        rom_file.read(reinterpret_cast<char*>(bank.data()), ROM_BANK_SIZE);
        std::size_t bytes_read = rom_file.gcount();
        if (bytes_read > 0) {
            rom.push_back(std::move(bank));
        }
    }

    // ROM bank handling
    rom_size = rom[0][ROM_SIZE_ADDRESS];
    if (rom.size() != (1 << (rom_size + 1))) {
        std::ostringstream rom_size_address;
        rom_size_address << std::hex << ROM_SIZE_ADDRESS;
        Log::error(
            "Cartridge header has incorrect ROM size value at " +
            rom_size_address.str()
        );
        exit(20);
    }

    // RAM bank handling
    try {
        ram_size = has_external_ram ?
            external_ram_lookup.at(rom[0][RAM_SIZE_ADDRESS]) : 0;
        reset();
    }
    catch(const std::out_of_range& e) {
        std::ostringstream ram_size_address;
        ram_size_address << std::hex << RAM_SIZE_ADDRESS;
        Log::error(
            "Cartridge header has invalid RAM size value at " +
            ram_size_address.str()
        );
        exit(21);
    }
}

// Clear RAM.
void BaseMbc::reset() {
    ram.resize(ram_size, std::array<uint8_t, RAM_BANK_SIZE>());
}