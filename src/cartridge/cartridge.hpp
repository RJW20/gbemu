#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <string>
#include <vector>
#include <cstdint>

class Cartridge {
public:
    Cartridge(std::string rom_path);
    ~Cartridge() {};

private:
    std::vector<uint8_t> rom;   // Vector to store ROM in
};

#endif