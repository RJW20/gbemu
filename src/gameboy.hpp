#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <string>
#include "cartridge/cartridge.hpp"
#include "interrupt_manager.hpp"
#include "timer.hpp"
#include "serial.hpp"
#include "mmu.hpp"
#include "cpu/cpu.hpp"

class GameBoy {
public:
    GameBoy(std::string rom_path) : cartridge(rom_path) {};
    ~GameBoy() {};

    void run();

private:
    Cartridge cartridge;
    InterruptManager interrupt_manager;
    Timer timer = Timer(&interrupt_manager);
    Serial serial = Serial(&interrupt_manager);
    Mmu mmu = Mmu(&cartridge, &interrupt_manager, &timer, &serial);
    Cpu cpu = Cpu(&interrupt_manager, &mmu);

    void tick();
};

#endif