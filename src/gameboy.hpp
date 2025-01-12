#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <string>
#include "cartridge/cartridge.hpp"
#include "interrupt_manager.hpp"
#include "timer.hpp"
#include "serial.hpp"
#include "dma.hpp"
#include "ppu.hpp"
#include "mmu.hpp"
#include "cpu/cpu.hpp"

class GameBoy {
public:
    GameBoy(std::string rom_path) : cartridge(rom_path) { dma.set_mmu(&mmu); };
    ~GameBoy() {};

    void run();

private:
    Cartridge cartridge;
    InterruptManager interrupt_manager;
    Timer timer{&interrupt_manager};
    Serial serial{&interrupt_manager};
    Dma dma{};
    Ppu ppu{&interrupt_manager};
    Mmu mmu{&cartridge, &interrupt_manager, &timer, &serial, &dma, &ppu};
    Cpu cpu{&interrupt_manager, &mmu};

    void tick();
};

#endif