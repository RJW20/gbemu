#ifndef PPU_HPP
#define PPU_HPP

#include <cstdint>
#include <cstddef>
#include <vector>
#include <deque>
#include "interrupt_manager.hpp"
#include "mmu.hpp"

/* Pixel Processing Unit 
 * */
class Ppu {
public:
    Ppu(InterruptManager* interrupt_manager, Mmu* mmu) :
        interrupt_manager(interrupt_manager), mmu(mmu) { reset(); };
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

private:
    InterruptManager* interrupt_manager;
    Mmu* mmu;

    static constexpr std::size_t VRAM_SIZE = 0x2000;    // 8 KB
    static constexpr std::size_t OAM_SIZE = 0x100;      // 256 B

    std::vector<uint8_t> vram;  // Video RAM
    std::vector<uint8_t> oam;   // Object attribute memory

    // Ppu mode
    enum class Mode : int {
        OAM_SCAN = 2, PIXEL_TRANSFER = 3, HBLANK = 0, VBLANK = 1
    };
    void set_mode(Mode mode);

    // Main loop variables
    Mode mode;
    uint16_t current_t_cycles;

    // Tick methods for each mode
    void oam_scan_tick();
    void pixel_transfer_tick();
    void hblank_tick();
    void vblank_tick();

    /* RGBA colour values, indexed by 2 bit pixel colour ID
     * 0 - Light-green
     * 1 - Green
     * 2 - Dark-green
     * 3 - Black */
    const uint32_t palette[4] = {
        0xE0F8D0FF, 0x88C070FF, 0x346856FF, 0x081820FF
    };

    // FIFO's
    uint32_t bgwin_fifo;
    uint32_t sprite_fifo; 

    // Underlying registers that are (partially) read-only
    uint8_t ly_;
    uint8_t stat_;

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