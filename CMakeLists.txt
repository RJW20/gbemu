cmake_minimum_required(VERSION 3.10)
project(gbemu)

set(CMAKE_CXX_STANDARD 23)

# Source files
set(SOURCES
    src/main.cpp
    src/logger.cpp
    src/gameboy.cpp
    src/cartridge/cartridge.cpp
    src/cartridge/mbc/base_mbc.cpp
    src/cartridge/mbc/no_mbc.cpp
    src/cartridge/mbc/mbc1.cpp
    src/cartridge/mbc/mbc2.cpp
    src/interrupt_manager.cpp
    src/timer.cpp
    src/serial.cpp
    src/joypad.cpp
    src/dma.cpp
    src/ppu/ppu_core.cpp
    src/ppu/oam_scanner/oam_scanner.cpp
    src/ppu/pixel_transferrer/pixel_transferrer.cpp
    src/ppu/ppu.cpp
    src/apu.cpp
    src/mmu.cpp
    src/exceptions.cpp
    src/cpu/cpu.cpp
    src/cpu/register.cpp
    src/cpu/basis.cpp
    src/cpu/opcodes.cpp
    src/screen.cpp
)

# SDL2 (for graphics, sound, and input)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(LIBS ${SDL2_LIBRARIES})

# Create executable
add_executable(gbemu ${SOURCES})
target_link_libraries(gbemu ${LIBS})