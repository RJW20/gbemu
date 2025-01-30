#ifndef PIXEL_FIFO_HPP
#define PIXEL_FIFO_HPP

#include <cstdint>
#include <array>

/* PaletteRegister
 * Enum containing information about which palette register a Pixel will derive
 * its colour from. */
enum class PaletteRegister : uint8_t {
    BGP = 0,
    OBP0 = 1,
    OBP1 = 2
};

/* Pixel 
 * Struct containing all information about an entry in a PixelFifo. */
struct Pixel {
    uint8_t colour_id : 2;
    PaletteRegister palette_register : 2;
    bool priority : 1;
};

/* Pixel First-In-First-Out
 * Data structure for storing Pixels.
 * Pixels are shifted out in the order they were pushed. */
class PixelFifo {
public:
    PixelFifo() { clear(); }

    void clear() { front_pointer = 0; end_pointer = 0; size = 0; }
    bool is_accepting_pixels() const { return size <= 8; }
    bool is_shifting_pixels() const { return size > 8; }
    bool is_empty() const { return !size; }
    void push(Pixel pixel) { data[end_pointer++] = pixel; size++; }
    Pixel shift() { return data[front_pointer++]; size--; }

private:
    std::array<Pixel, 0x10> data;
    uint8_t front_pointer : 4;
    uint8_t end_pointer : 4;
    uint8_t size;
};

#endif