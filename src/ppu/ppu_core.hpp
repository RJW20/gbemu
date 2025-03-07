#ifndef PPU_CORE_HPP
#define PPU_CORE_HPP

#include <cstdint>
#include <array>
#include <deque>
#include "oam_scanner/oam_object.hpp"
#include "../interrupt_manager.hpp"

/* Pixel Processing Unit Core 
 * Contains all the underlying core functionality of the PPU. */
class PpuCore {
public:
    PpuCore() {}
    PpuCore(InterruptManager* interrupt_manager) :
        interrupt_manager(interrupt_manager) {}

    // VRAM/OAM access
    uint8_t read_vram(uint16_t address) const;
    void write_vram(uint16_t address, uint8_t value);
    uint8_t read_oam(uint16_t address) const;
    void write_oam(uint16_t address, uint8_t value);

    // Public read/write registers
    uint8_t lyc;
    uint8_t scy;
    uint8_t scx;
    uint8_t wy;
    uint8_t wx;
    uint8_t bgp;
    uint8_t obp0;
    uint8_t obp1;

private:
    static constexpr uint16_t VRAM_SIZE = 0x2000;    // 8 KB
    static constexpr uint16_t OAM_SIZE = 0x100;      // 160 B

protected:
    InterruptManager* interrupt_manager;

    std::array<uint8_t, VRAM_SIZE> vram;    // Video RAM
    std::array<uint8_t, OAM_SIZE> oam;      // Object attribute memory

    // Variables used during multiple modes
    uint16_t mode_t_cycles;
    uint16_t scanline_t_cycles;
    std::deque<OamObject> scanline_objects;
    bool window_present_on_scanline;    // Decided at start of scanline

    // Underlying (partially) read-only registers
    uint8_t lcdc_;
    uint8_t ly_;
    uint8_t stat_;

    // Methods for reading register flags
    bool lcd_enabled() const { return (lcdc_ >> 7) & 1; }
    uint16_t window_tile_map_address() const {
        return (lcdc_ >> 6) & 1 ? 0x1C00 : 0x1800;
    }
    bool window_enabled() const {return (lcdc_ >> 5) & 1; }
    bool bgwin_unsigned_addressing() const { return (lcdc_ >> 4) & 1; }
    uint16_t background_tile_map_address() const {
        return (lcdc_ >> 3) & 1 ? 0x1C00 : 0x1800;
    }
    bool double_object_height() const { return (lcdc_ >> 2) & 1; }
    bool objects_enabled() const { return (lcdc_ >> 1) & 1;} 
    bool bgwin_enabled() const { return lcdc_ & 1; }
    bool lyc_interrupt_requested() const { return (stat_ >> 6) & 1; }
    bool oam_scan_interrupt_requested() const { return (stat_ >> 5) & 1; }
    bool vblank_interrupt_requested() const { return (stat_ >> 4) & 1; }
    bool hblank_interrupt_requested() const { return (stat_ >> 3) & 1; }
};

#endif