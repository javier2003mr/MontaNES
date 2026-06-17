#ifndef PPUCLASS
#define PPUCLASS

#include "CPU.hpp"
#include "Cartridge.hpp"

#define PPU_RAM_SIZE 65536
#define SPR_RAM_SIZE 256

class PPU {
private:
    CPU *nes_cpu;
    Cartridge *nes_cartridge;
    
    //unsigned char ppu_memory[PPU_RAM_SIZE];
    unsigned char spr_ram[SPR_RAM_SIZE];
    unsigned char vram[0x4000];  // 16KB VRAM
    unsigned char palette_ram[0x20];  // 32 bytes palette RAM
    
    // Internal registers
    unsigned short v;  // Current VRAM address (15 bits)
    unsigned short t;  // Temporary VRAM address (15 bits)
    unsigned char x;   // Fine X scroll (3 bits)
    bool w;           // First/second write toggle
    bool odd_frame;   // For frame skipping

    unsigned char ppu_ctrl;    // $2000
    unsigned char ppu_mask;    // $2001
    unsigned char ppu_status;  // $2002
    unsigned char oam_addr;    // $2003
    unsigned char oam_data;    // $2004 (read/write through OAM)
    unsigned char ppu_scroll;  // $2005
    unsigned char ppu_addr;    // $2006
    unsigned char ppu_data;    // $2007 (read/write through VRAM)
    unsigned char data_buffer; // Internal PPU read buffer
    
    // Rendering buffers
    unsigned int frame_buffer[256 * 240];  // RGB frame buffer
    
    // Timing counters
    int scanline;      // 0-261
    int cycle;         // 0-340
    
    // Fetch registers
    unsigned char nt_byte, at_byte;
    unsigned char bg_low, bg_high;
    unsigned char spr_low, spr_high;
    
    // Shift registers
    unsigned short bg_pattern_low;
    unsigned short bg_pattern_high;
    unsigned short bg_palette_low;
    unsigned short bg_palette_high;
    
    // Sprite rendering
    unsigned char sprite_positions[8];
    unsigned char sprite_patterns[8];
    unsigned char sprite_attributes[8];
    unsigned char sprite_oam_indices[8];
    unsigned char sprite_pattern_low[8];
    unsigned char sprite_pattern_high[8];
    int sprite_count;
    
public:
    PPU(CPU *cpu, Cartridge *cart);
    
    // Register access
    unsigned char getPPUCTRL();
    unsigned char getPPUMASK();
    unsigned char getPPUSTATUS();
    unsigned char getOAMADDR();
    unsigned char getOAMDATA();
    unsigned char getPPUSCROLL();
    unsigned char getPPUADDR();
    unsigned char getPPUDATA();
    unsigned char * getVRAM();
    
    void setPPUCTRL(unsigned char value);
    void setPPUMASK(unsigned char value);
    void setPPUSTATUS(unsigned char value);
    void setOAMADDR(unsigned char value);
    void setOAMDATA(unsigned char value);
    void setPPUSCROLL(unsigned char value);
    void setPPUADDR(unsigned char value);
    void setPPUDATA(unsigned char value);
    void setOAMDMA(unsigned char value);
    
    // Memory access
    unsigned char ppuRead(unsigned short addr);
    void ppuWrite(unsigned short addr, unsigned char value);
    
    // Rendering functions
    void runCycle();
    void renderPixel();
    void fetchBackgroundData();
    void evaluateSprites();
    unsigned int readPalette(unsigned char index);
    unsigned short mirrorNametableAddr(unsigned short addr);
    
    // Frame control
    void reset();
    void startFrame();
    void endFrame();
    bool isFrameReady();
    unsigned int* getFrameBuffer();
    
    // Status helpers
    bool isRenderingEnabled();
    bool isSpriteZeroHit();
    void setSpriteZeroHit(bool hit);
    
    // Debug
    void dumpVRAM();
    void dumpPalette();
};

#endif