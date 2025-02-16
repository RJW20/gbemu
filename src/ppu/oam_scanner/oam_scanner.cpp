#include <cstdint>
#include "oam_scanner.hpp"

// Initialise variables for a new scanline.
void OamScanner::new_oam_scan() {
    scanline_t_cycles = 0;
    scan_index = 0;
    scanline_objects.resize(0);
    bgwin_pixels_to_discard = scx & 0x7;
    window_present_on_scanline = (ly_ >= wy);
}

/* Carry out 1 OAM_SCAN t-cycle.
 * Reads 4 bytes from the OAM and fully determines an OamObject every 2 t-
 * cycles.
 * If the object is visible on the current scanline, it is added to
 * scanline_objects to be used during PIXEL_TRANSFER (up to 10 maximum). */
void OamScanner::oam_scan_tick() {

    // Reads objects as off screen if DMA is active

    if ((mode_t_cycles & 0x1) && scanline_objects.size() < 10) {
        scan_object.y = vram[scan_index++];
        scan_object.x = vram[scan_index++];
        if (scan_object.x > 0 && scan_object.y - 16 <= ly_ &&
            scan_object.y - 16 + 8 * (1 + double_object_height()) > ly_) {
            scan_object.tile_id = vram[scan_index++];
            const uint8_t attributes = vram[scan_index++];
            scan_object.priority = (attributes >> 7) & 1;
            scan_object.y_flip = (attributes >> 6) & 1;
            scan_object.x_flip = (attributes >> 5) & 1;
            scan_object.pallette = (attributes >> 4) & 1;
            scanline_objects.push_back(scan_object);
        }
    }
}