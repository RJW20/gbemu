#ifndef PPU_HPP
#define PPU_HPP

#include <cstdint>
#include <cstddef>
#include <array>
#include <vector>
#include "interrupt_manager.hpp"
#include "mmu.hpp"

/* Pixel Processing Unit 
 * */
class Ppu {
public:
    Ppu(InterruptManager* interrupt_manager) :
        interrupt_manager(interrupt_manager) { reset(); };
    ~Ppu() {};

    void reset();
    void tick();

    // Public read/write registers
    uint8_t lcdc;
    uint8_t lyc;
    uint8_t scy;
    uint8_t scx;
    uint8_t wy;
    uint8_t wx;
    uint8_t bgp;
    uint8_t obp0;
    uint8_t obp1;

    // Registers that are (partially) read-only
    uint8_t ly() const;
    uint8_t stat() const;
    void set_stat(uint8_t value);

    // Pixel buffer
    static constexpr uint8_t SCREEN_WIDTH = 160;
    static constexpr uint8_t SCREEN_HEIGHT = 144;
    std::array<uint32_t, SCREEN_WIDTH * SCREEN_HEIGHT> pixel_buffer;

private:
    InterruptManager* interrupt_manager;

    static constexpr std::size_t VRAM_SIZE = 0x2000;    // 8 KB
    static constexpr std::size_t OAM_SIZE = 0x9F;       // 160 B
    std::array<uint8_t, VRAM_SIZE> vram;    // Video RAM
    std::array<uint8_t, OAM_SIZE> oam;      // Object attribute memory

    // Ppu modes
    enum class Mode : int {
        OAM_SCAN = 2, PIXEL_TRANSFER = 3, HBLANK = 0, VBLANK = 1
    };
    void set_mode(Mode mode);
    static constexpr uint8_t OAM_T_CYCLES = 80;
    //enum class PixelFetcherSource {BACKGROUND, WINDOW, OBJECT};
    enum class PixelFetcherMode {PUSH, TILE_ID, TILE_ROW_LOW, TILE_ROW_HIGH};
    static constexpr uint8_t SCANLINE_T_CYCLES = 456;
    static constexpr uint8_t VBLANK_SCANLINES = 10;

    // Main loop variables
    Mode mode;
    uint8_t current_t_cycles;
    //uint8_t oam_scan_index;
    //std::vector<uint8_t> scanline_object_indexes;
    uint8_t pixels_to_discard;  // Read at start of scanline
    //PixelFetcherSource pixel_fetcher_source;
    //uint8_t wly;
    PixelFetcherMode pixel_fetcher_mode;
    uint8_t fetcher_cycles;
    uint8_t lx;
    uint8_t current_scanline_tile;
    uint8_t tile_id;
    uint16_t tile_row;
    uint32_t bgwin_fifo;    // 16x2 bit pixels
    //uint32_t sprite_fifo;
    uint8_t bgwin_fifo_pointer; // 0 points to bits 31-30
    //uint8_t sprite_fifo_pointer;

    // Tick methods for each mode
    void oam_scan_tick();
    void pixel_transfer_tick();
    void hblank_tick();
    void vblank_tick();

    // Pixel transfer mode and helper methods
    void reset_fetcher();
    uint8_t fetch_background_tile_id(uint8_t x_offset);
    //uint8_t fetch_window_tile_id();
    uint8_t fetch_tile_row(uint8_t tile_id, uint8_t row, bool low);

    /* RGBA colour values, indexed by 2 bit pixel colour ID
     * 0 - Light-green
     * 1 - Green
     * 2 - Dark-green
     * 3 - Black */
    static constexpr std::array<uint32_t,4> palette = {
        0xE0F8D0FF, 0x88C070FF, 0x346856FF, 0x081820FF
    };

    // Underlying registers that are (partially) read-only
    uint8_t _ly;
    uint8_t _stat;

    // Methods for reading register flags
    bool lcd_enabled() const;
    uint16_t window_tile_map_address() const;
    bool window_enabled() const;
    bool bgwin_unsigned_addressing() const;
    uint16_t background_tile_map_address() const;
    bool double_object_height() const;
    bool object_enabled() const;
    bool bgwin_enabled() const;
    bool lyc_interrupt_requested() const;
    bool oam_scan_interrupt_requested() const;
    bool vblank_interrupt_requested() const;
    bool hblank_interrupt_requested() const;
};

#endif