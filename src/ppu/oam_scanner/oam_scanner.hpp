#ifndef OAM_SCANNER_HPP
#define OAM_SCANNER_HPP

#include <cstdint>
#include "../ppu_core.hpp"
#include "oam_object.hpp"

class Dma;

/* OAM Scanner 
 * Scans OAM during the beginning of a scanline for OAM Objects on that
 * scanline. */
class OamScanner : virtual public PpuCore {
public:
    OamScanner() {}

    void set_dma(Dma* _dma) { dma = _dma; }

private:
    Dma* dma;

    uint8_t scan_index;
    OamObject scan_object;
    
protected:
    static constexpr uint8_t OAM_T_CYCLES = 80;

    void new_oam_scan();
    void oam_scan_tick();
};

#endif