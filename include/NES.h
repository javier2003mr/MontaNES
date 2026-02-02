#include <cstdint>
#include "Cartridge.hpp"
#include "CPU.h"
#include "joypad.h"
#include "PPU.hpp"

// In your main emulator class
class Emulator {
private:
    CPU cpu;
    PPU ppu;
    Cartridge cartridge;
    
public:
    Emulator() : ppu(&cpu, &cartridge) {
        // Initialize
    }
    
    void runFrame() {
        int cpu_cycles = 0;
        
        while (cpu_cycles < 29780) {  // Cycles per frame
            // Execute one CPU instruction
            int cycles = cpu.emulationCycle();
            cpu_cycles += cycles;
            
            // Run PPU for 3 cycles per CPU cycle
            for (int i = 0; i < cycles * 3; i++) {
                ppu.runCycle();
            }
        }
        
        // At this point, a frame is complete
        // You can now render the frame_buffer to your HaikuOS window
    }
};