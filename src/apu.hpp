#ifndef APU_HPP
#define APU_HPP

#include <cstdint>
#include <array>
#include <string>

/* Audio Processing Unit
 * Implements no functionality beyond containing 21 audio registers and 16
 * bytes of wave RAM. */
class Apu {
public:
    Apu() { reset(); } 

    void reset();

    uint8_t read_wave_ram(uint8_t address) const;
    void write_wave_ram(uint8_t address, uint8_t value);

    // Sound channel 1
    uint8_t nr10() const { return nr10_ & 0x7F; }
    uint8_t nr11() const { return nr11_ & 0xC0; }
    uint8_t nr12() const { return nr12_; }
    uint8_t nr13() const { return nr13_; }
    uint8_t nr14() const { return nr14_ & 0x40; }
    void set_nr10(uint8_t value) { nr10_ = value; }
    void set_nr11(uint8_t value) { nr11_ = value; }
    void set_nr12(uint8_t value) { nr12_ = value; }
    void set_nr13(uint8_t value) { nr13_ = value; }
    void set_nr14(uint8_t value) { nr14_ = value; }

    // Sound channel 2
    uint8_t nr21() const { return nr21_ & 0xC0; }
    uint8_t nr22() const { return nr22_; }
    uint8_t nr23() const { return nr23_; }
    uint8_t nr24() const { return nr24_ & 0x40; }
    void set_nr21(uint8_t value) { nr21_ = value; }
    void set_nr22(uint8_t value) { nr22_ = value; }
    void set_nr23(uint8_t value) { nr23_ = value; }
    void set_nr24(uint8_t value) { nr24_ = value; }

    // Sound channel 3
    uint8_t nr30() const { return nr30_ & 0x80; }
    uint8_t nr31() const { return 0xFF; }
    uint8_t nr32() const { return nr32_ & 0x60; }
    uint8_t nr33() const { return 0xFF; }
    uint8_t nr34() const { return nr34_ & 0xC7; }
    void set_nr30(uint8_t value) { nr30_ = value; }
    void set_nr31(uint8_t value) { nr31_ = value; }
    void set_nr32(uint8_t value) { nr32_ = value; }
    void set_nr33(uint8_t value) { nr33_ = value; }
    void set_nr34(uint8_t value) { nr34_ = value; }

    // Sound channel 4
    uint8_t nr41() const { return 0xFF; }
    uint8_t nr42() const { return nr42_; }
    uint8_t nr43() const { return nr43_; }
    uint8_t nr44() const { return nr44_ & 0x40; }
    void set_nr41(uint8_t value) { nr41_ = value; }
    void set_nr42(uint8_t value) { nr42_ = value; }
    void set_nr43(uint8_t value) { nr43_ = value; }
    void set_nr44(uint8_t value) { nr44_ = value; }

    // Global control
    uint8_t nr50() const { return nr50_; }
    uint8_t nr51() const { return nr51_; }
    uint8_t nr52() const { return nr52_ & 0x8F; }
    void set_nr50(uint8_t value) { nr50_ = value; }
    void set_nr51(uint8_t value) { nr51_ = value; }
    void set_nr52(uint8_t value) { nr52_ = (nr52_ & 0xF) | value & 0x80; }

    std::string representation() const;

private:
    // Sound channel 1
    uint8_t nr10_;
    uint8_t nr11_;
    uint8_t nr12_;
    uint8_t nr13_;
    uint8_t nr14_;

    // Sound channel 2
    uint8_t nr21_;
    uint8_t nr22_;
    uint8_t nr23_;
    uint8_t nr24_;

    // Sound channel 3
    uint8_t nr30_;
    uint8_t nr31_;
    uint8_t nr32_;
    uint8_t nr33_;
    uint8_t nr34_;

    // Sound channel 4
    uint8_t nr41_;
    uint8_t nr42_;
    uint8_t nr43_;
    uint8_t nr44_;

    // Global control
    uint8_t nr50_;
    uint8_t nr51_;
    uint8_t nr52_;

    std::array<uint8_t, 0x10> wave_ram;
};

#endif