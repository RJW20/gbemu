#ifndef LOOKUP_HPP
#define LOOKUP_HPP

#include <cstdint>
#include <unordered_map>
#include <functional>
#include <memory>
#include <fstream>
#include "base_mbc.hpp"
#include "no_mbc.hpp"
#include "mbc1.hpp"
#include "mbc2.hpp"

// Maps to constructor for MBC in the cartridge
const std::unordered_map<
    uint8_t,
    std::function<std::unique_ptr<BaseMbc>(std::ifstream&)>
> mbc_lookup = {
    // No MBC
    {0x0, [](std::ifstream& rom_file) {
        return std::make_unique<NoMbc>(rom_file);
    }},

    // MBC1
    {0x1, [](std::ifstream& rom_file) {
        return std::make_unique<Mbc1>(rom_file, false);
    }},
    {0x2, [](std::ifstream& rom_file) {
        return std::make_unique<Mbc1>(rom_file, true);
    }},
    {0x3, [](std::ifstream& rom_file) {
        return std::make_unique<Mbc1>(rom_file, true);
    }},

    // MBC2
    {0x5, [](std::ifstream& rom_file) {
        return std::make_unique<Mbc2>(rom_file);
    }},
    {0x6, [](std::ifstream& rom_file) {
        return std::make_unique<Mbc2>(rom_file);
    }},
};

// Number of external 8KB banks in the cartridge
const std::unordered_map<uint8_t, uint8_t> external_ram_lookup = {
    {0x00, 0}, 
    {0x01, 1}, // Only supposed to be 2KB but allocate 8KB
    {0x02, 1},
    {0x03, 4},
    {0x04, 16},
    {0x05, 8}
};

#endif