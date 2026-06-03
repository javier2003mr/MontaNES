#include "NES.h"
#include "Cartridge.hpp"
#include "CPU.h"
#include "PPU.hpp" // Using the more complete PPU implementation
#include <iostream>

NES::NES() {
    // Constructor is intentionally left light.
    // Initialization happens when a cartridge is loaded.
    cpu = nullptr;
    ppu = nullptr;
    cartridge = nullptr;
    apu = nullptr;
    joypad = new Joypad(); // Initialize Joypad
    apu_frame_sequencer_timer = 0;
}

NES::~NES() {
    // std::unique_ptr will automatically handle the cleanup of cpu, ppu, and cartridge.
    if (cpu != nullptr) delete cpu;
    if (ppu != nullptr) delete ppu;
    if (cartridge != nullptr) delete cartridge; // Assuming cartridge is also dynamically allocated
    if (joypad != nullptr) delete joypad; // Delete Joypad
    if (apu != nullptr) {
        apu->stop(); // Stop the APU before deleting it
        delete apu;
    }
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
    cpu = new CPU;
    ppu = new PPU (cpu, cartridge);
    apu = new APU;
    cpu->connectPPU(ppu);
    cpu->connectJoypad(joypad);
    cpu->connectCartridge(cartridge);
    cpu->connectAPU(apu);

    // --- Load PRG-ROM into CPU memory ---
    unsigned char* prg_rom = cartridge->getPRGROM();
    unsigned char prg_rom_size = cartridge->getPRGROMSize(); // In 16KB units
    
    printf("PRG ROM Size: %d * 16384\n", prg_rom_size);

    // Note: This currently only handles NROM (mapper 0) with 16KB or 32KB.
    // More complex mappers would require more sophisticated memory mapping here.

    // Reset the system to its initial state before loading CHR ROM.
    reset();

    // Start the APU after everything is initialized and reset
    apu->start();

    // --- Load CHR-ROM into PPU memory ---
//    unsigned char* chr_rom = cartridge->getCHRROM();
    unsigned char chr_rom_size = cartridge->getCHRROMSize(); // In 8KB units
    printf("CHR ROM Size: %d * 8192\n", chr_rom_size);

    cartridgeLoaded = true;

    return true;
}

void NES::reset() {

    cpu->reset();

    // Reset the PPU.
    ppu->reset();
    
    // Reset the APU.
    apu->reset();

    apu_frame_sequencer_timer = 0;
}

void NES::runFrame() {

    if (!cartridgeLoaded) return;

    //Numero de ciclos para NTSC
    int cpu_cycles_this_frame = 0;
    const int total_cycles_per_frame = 29781;

    int current_apu_step = 0;

    while (cpu_cycles_this_frame < total_cycles_per_frame) {

        // The CPU executes one instruction and returns the number of cycles it took.
        int cycles = cpu->emulationCycle();
        cpu_cycles_this_frame += cycles;

        // The PPU runs 3 times for every 1 CPU cycle.
        for (int i = 0; i < cycles * 3; i++) {
            ppu->runCycle();
        }

        // APU Frame Sequencer timing
        apu_frame_sequencer_timer += cycles;

        if (!apu->getStepMode()) { // 4-step sequence
            if ((apu_frame_sequencer_timer >= APU_FRAME_COUNTER_4STEP_0 && current_apu_step == 0) ||
                (apu_frame_sequencer_timer >= APU_FRAME_COUNTER_4STEP_1 && current_apu_step == 1) ||
                (apu_frame_sequencer_timer >= APU_FRAME_COUNTER_4STEP_2 && current_apu_step == 2) ||
                (apu_frame_sequencer_timer >= APU_FRAME_COUNTER_4STEP_3 && current_apu_step == 3)) {
                ++current_apu_step;
                apu->step();
            }
            if (apu_frame_sequencer_timer >= APU_FRAME_COUNTER_4STEP_RESET) {

                apu_frame_sequencer_timer = 0;
                current_apu_step = 0;
            }
        } else { // 5-step sequence
            if ((apu_frame_sequencer_timer >= APU_FRAME_COUNTER_5STEP_0 && current_apu_step == 0) ||
                (apu_frame_sequencer_timer >= APU_FRAME_COUNTER_5STEP_1 && current_apu_step == 1) ||
                (apu_frame_sequencer_timer >= APU_FRAME_COUNTER_5STEP_2 && current_apu_step == 2) ||
                (apu_frame_sequencer_timer >= APU_FRAME_COUNTER_5STEP_3 && current_apu_step == 3)) {
                ++current_apu_step;
                apu->step();
            }
            if (apu_frame_sequencer_timer >= APU_FRAME_COUNTER_5STEP_RESET) {
                apu->step();
                apu_frame_sequencer_timer = 0;
                current_apu_step = 0;
            }
        }
    }

    //Ya se ha generado un frame
}

PPU* NES::getPPU() const {
    return ppu;
}

CPU* NES::getCPU() const {
    return cpu;
}

Cartridge* NES::getCartridge() const {
    return cartridge;
}

Joypad* NES::getJoypad() const {
    return joypad;
}

APU * NES :: getAPU() const{
    return apu;
}

bool NES::isCartridgeLoaded() const {
    return cartridgeLoaded;
}
