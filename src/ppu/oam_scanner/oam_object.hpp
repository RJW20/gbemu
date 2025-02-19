#ifndef OAM_OBJECT_HPP
#define OAM_OBJECT_HPP

#include <cstdint>
#include <string>
#include <sstream>

/* OAM Object
 * Struct containing 4 bytes worth of data about an OAM object. */
struct OamObject {
    uint8_t y;  // Vertical position on the screen + 16
    uint8_t x;  // Horizontal position on the screen + 8 
    uint8_t tile_id;    // Specifies the tile in VRAM
    uint8_t priority : 1;   // 0 = yes, 1 = no (bg/win ids 1-3 draw over this)
    uint8_t y_flip : 1;     // 0 = normal, 1 = vertically mirrored.
    uint8_t x_flip : 1;     // 0 = normal, 1 = horizontally mirrored.
    uint8_t pallette : 1;   // 0 = OBP0, 1 = OBP1.

    std::string representation() const {
        std::ostringstream repr;
        repr << "Object:"
            << " x = " << static_cast<int>(x)
            << " y = " << static_cast<int>(y)
            << " id = " << static_cast<int>(tile_id)
            << " priority = " << static_cast<int>(priority);
        return repr.str();
    }
};

#endif