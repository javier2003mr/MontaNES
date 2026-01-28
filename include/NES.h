#include <cstdint>
#include "cartridge.h"
#include "CPU.h"
#include "joypad.h"
#include "PPU.hpp"

class NES {
    public:
        CPU cpu;
        PPU ppu;
        Cartridge cartridge;
        Joypad joypad;
        bool loaded = false;

        NES() : cpu(*this), ppu(*this), apu(*this), cartridge(*this) {}
        void load(const char* filename);
        void run_frame();
};