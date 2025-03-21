#ifndef PPU_HPP
#define PPU_HPP

#include <cstdint>
#include <string>
#include "ppu_core.hpp"
#include "oam_scanner/oam_scanner.hpp"
#include "pixel_transferrer/pixel_transferrer.hpp"
#include "../interrupt_manager.hpp"

/* Pixel Processing Unit 
 * Uses tiles in VRAM and objects in OAM to push RGBA values to a pixel buffer
 * for use as an SDL2 texture. */
class Ppu : public OamScanner, public PixelTransferrer {
public:
    Ppu(InterruptManager* interrupt_manager) : PpuCore(interrupt_manager)
        { reset(); }

    void reset();
    void tick();

    // (Partially) read-only registers access
    uint8_t lcdc() const { return lcdc_; }
    uint8_t ly() const;
    uint8_t stat() const;
    void set_lcdc(uint8_t value);
    void set_ly(uint8_t value) const;
    void set_stat(uint8_t value) { stat_ = value & 0x78; }

    bool ready_to_render;

    std::string representation() const;

private:
    // Ppu mode
    enum class Mode : uint8_t {
        OAM_SCAN = 2, PIXEL_TRANSFER = 3, HBLANK = 0, VBLANK = 1
    };
    Mode mode;
    bool mode_change;
    Mode pending_mode;
    void set_mode(Mode new_mode);

    // Tick methods for each mode
    void oam_scan_tick();
    void pixel_transfer_tick();
    void hblank_tick();
    void vblank_tick();

    void (Ppu::* mode_tick)();

    static constexpr uint16_t SCANLINE_T_CYCLES = 456;
    static constexpr uint8_t VBLANK_SCANLINES = 10;
};

#endif