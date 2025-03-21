#include <cstdint>
#include "oam_scanner.hpp"
#include "../../interrupt_manager.hpp"
#include "../../dma.hpp"

/* Initialise variables for a new scanline.
 * Increments ly_ or resets it depending on if the previous mode was HBLANK or
 * VBLANK. */
void OamScanner::new_oam_scan() {
    scanline_t_cycles = 0;
    scan_index = 0;
    scanline_objects.resize(0);

    if (++ly_ > SCREEN_HEIGHT) {
        ly_ = 0;
    }
}

/* Carry out 1 OAM_SCAN t-cycle.
 * Reads 4 bytes from the OAM and fully determines an OamObject every 2 t-
 * cycles.
 * If the object is visible on the current scanline, it is added to
 * scanline_objects to be used during PIXEL_TRANSFER (up to 10 maximum). */
void OamScanner::oam_scan_tick() {

    if (dma->transfer_in_progress) {
        scan_index += 2;
        return;
    }

    if ((scanline_t_cycles & 0x1) && scanline_objects.size() < 10) {
        scan_object.y = oam[scan_index];
        scan_object.x = oam[scan_index + 1];
        if (scan_object.x > 0 && scan_object.y - 16 <= ly_ &&
            scan_object.y - 16 + 8 * (1 + double_object_height()) > ly_) {
            scan_object.tile_id = oam[scan_index + 2];
            const uint8_t attributes = oam[scan_index + 3];
            scan_object.priority = (attributes >> 7) & 1;
            scan_object.y_flip = (attributes >> 6) & 1;
            scan_object.x_flip = (attributes >> 5) & 1;
            scan_object.pallette = (attributes >> 4) & 1;
            scanline_objects.push_back(scan_object);
        }
        scan_index += 4;
    }
}