#include "NES.h"
#include "Cartridge.hpp"
#include "CPU.h"
#include "PPU2.hpp" // Using the more complete PPU implementation
#include <iostream>

NES::NES() {
    // Constructor is intentionally left light.
    // Initialization happens when a cartridge is loaded.
}

NES::~NES() {
    // std::unique_ptr will automatically handle the cleanup of cpu, ppu, and cartridge.
}

bool NES::loadCartridge(const std::string& path) {
    // Create the cartridge first.
    //cartridge = std::make_unique<Cartridge>();
    cartridge = new Cartridge;
    cartridge->loadROM((char*)path.c_str());

    // Basic check: In a real scenario, loadROM should return success/failure.
    if (cartridge->getPRGROM() == nullptr) {
        std::cerr << "Failed to load ROM: " << path << std::endl;
        cartridgeLoaded = false;
        return false;
    }

    // With a valid cartridge, create the other components.
    //cpu = std::make_unique<CPU>();
    //ppu = std::make_unique<PPU>(cpu.get(), cartridge.get());
    cpu = new CPU;
    ppu = new PPU (cpu, cartridge);
    cpu->connectPPU(ppu);

    // --- Load PRG-ROM into CPU memory ---
    unsigned char* prg_rom = cartridge->getPRGROM();
    unsigned char prg_rom_size = cartridge->getPRGROMSize(); // In 16KB units

    if (prg_rom_size == 1) { // 16KB PRG-ROM
        // Mirror the 16KB chunk at 0x8000 and 0xC000
        for (int i = 0; i < 16384; i++) {
            cpu->setMemoryValue(0x8000 + i, prg_rom[i]);
            cpu->setMemoryValue(0xC000 + i, prg_rom[i]);
        }
    } else if (prg_rom_size == 2) { // 32KB PRG-ROM
        // Load the full 32KB chunk from 0x8000 to 0xFFFF
        for (int i = 0; i < 32768; i++) {
            cpu->setMemoryValue(0x8000 + i, prg_rom[i]);
        }
    }
    // Note: This currently only handles NROM (mapper 0) with 16KB or 32KB.
    // More complex mappers would require more sophisticated memory mapping here.

    cartridgeLoaded = true;
    
    // Reset the system to its initial state.
    reset();

    return true;
}

void NES::reset() {
    if (!cartridgeLoaded) return;

    // Reset CPU state (constructor handles most of this) and set Program Counter.
    // The PC is initialized to the 16-bit address stored at the reset vector ($FFFC).
    //cpu->setPC(cpu->getMemoryValue(0xFFFC) | (cpu->getMemoryValue(0xFFFD) << 8));
    //cpu->setA(0);
    //cpu->setX(0);
    //cpu->setY(0);
    //cpu->setSP(0xFD);
    //cpu->setP(0x24);
    cpu->reset();


    // Reset the PPU.
    ppu->reset();

    // Set initial PPU registers
    cpu->setMemoryValue(0x2000, 0x80); // PPUCTRL: NMI enable, 8x8 sprites, BG/Sprite pattern table $0000, VRAM increment +1, Nametable $2000
    cpu->setMemoryValue(0x2001, 0x18); // PPUMASK: Enable background and sprites
}

void NES::runFrame() {
    if (!cartridgeLoaded) return;

    // This loop runs the emulator for a duration equivalent to one NTSC frame.
    // The number of CPU cycles per frame is approximately 29781.
    int cpu_cycles_this_frame = 0;
    const int total_cycles_per_frame = 29781;

    while (cpu_cycles_this_frame < total_cycles_per_frame) {
        // The CPU executes one instruction and returns the number of cycles it took.
        int cycles = cpu->emulationCycle();
        cpu_cycles_this_frame += cycles;

        // The PPU runs 3 times for every 1 CPU cycle.
        for (int i = 0; i < cycles * 3; i++) {
            ppu->runCycle();
        }
    }
    // At the end of this loop, the PPU's framebuffer has a complete image.
}

// --- Accessors ---

PPU* NES::getPPU() const {
    //return ppu.get();
    return ppu;
}

CPU* NES::getCPU() const {
    //return cpu.get();
    return cpu;
}

Cartridge* NES::getCartridge() const {
    //return cartridge.get();
    return cartridge;
}

bool NES::isCartridgeLoaded() const {
    return cartridgeLoaded;
}
