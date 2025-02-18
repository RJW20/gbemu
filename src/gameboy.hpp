#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <string>
#include "cartridge/cartridge.hpp"
#include "interrupt_manager.hpp"
#include "timer.hpp"
#include "serial.hpp"
#include "joypad.hpp"
#include "ppu/ppu.hpp"
#include "dma.hpp"
#include "apu.hpp"
#include "mmu.hpp"
#include "cpu/cpu.hpp"
#include "screen.hpp"

// GameBoy
class GameBoy {
public:
    GameBoy(std::string rom_path) : cartridge(rom_path) { 
        ppu.set_dma(&dma); dma.set_mmu(&mmu);
    }

    void run();

    std::string representation() const;

private:
    Cartridge cartridge;
    InterruptManager interrupt_manager;
    Timer timer{&interrupt_manager};
    Serial serial{&interrupt_manager};
    Joypad joypad{&interrupt_manager};
    Ppu ppu{&interrupt_manager};
    Dma dma{&ppu};
    Apu apu;
    Mmu mmu{
        &cartridge, &interrupt_manager, &timer, &serial, &joypad, &ppu, &dma,
        &apu
    };
    Cpu cpu{&interrupt_manager, &mmu};
    Screen screen{cartridge.title, &ppu};

    static constexpr int SIXTY_FPS = 70224;     // Ticks per frame render
    int ticks = 0;

    void tick();
    bool power_off() const;
};

#endif