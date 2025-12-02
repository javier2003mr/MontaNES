#define CPU_RAM_SIZE 65536

class NES {
    private:
        unsigned char cpu_memory[CPU_RAM_SIZE];
        ppu_memory;
        ObjectAttributeMemory;

        CPU cpu;
        PPU ppu;
        APU apu;

        Cartridge cartridge;

        Interrupts interrupts;

        MMU mmu;

        GUI gui;

        Joypad joypads[NES_NUM_JOYPADS];
};