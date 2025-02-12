#include <cstdint>
#include "register.hpp"

// Set registers to their post boot ROM values.
void Register::reset() {
    set_af(0x01B0);
    set_bc(0x0013);
    set_de(0x00D8);
    set_hl(0x014D);
    pc = 0x100;
    sp = 0xFFFE;
}

/* Return the flags register.
 * Has the following structure:
 * - Bit 7 = flag_z
 * - Bit 6 = flag_n
 * - Bit 5 = flag_h
 * - Bit 4 = flag_c
 * - Bits 0-3 = 0 */
uint8_t Register::f() {
    return (flag_z << 7) | (flag_n << 6) | (flag_h << 5) | (flag_c << 4);
}

/* Set the flags that make up the flags register.
 * Outputs to std::cerr if the trailing 4 bits in the given value are non-
 * zero. */
void Register::set_f(uint8_t value) {
    flag_z = value >> 7;
    flag_n = (value >> 6) & 1;
    flag_h = (value >> 5) & 1;
    flag_c = (value >> 4) & 1;
}