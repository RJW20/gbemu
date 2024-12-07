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
    file.seekg(0, std::ios::beg);
    rom.resize(size);

    // Read the file into the rom vector
    if (!file.read(reinterpret_cast<char*>(rom.data()), size)) {
        std::cerr << "Failed to read ROM: " << rom_path << std::endl;
        exit(0);
    }
    file.close();
}