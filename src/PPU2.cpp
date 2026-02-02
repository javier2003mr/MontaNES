#include <stdio.h>
#include "../include/PPU.hpp"

// Constructor
PPU::PPU(CPU *cpu, Cartridge *cart) : nes_cpu(cpu), nes_cartridge(cart) {
    reset();
}

// Reset PPU to initial state
void PPU::reset() {
    v = 0;
    t = 0;
    x = 0;
    w = false;
    odd_frame = false;
    
    scanline = 261;  // Pre-render scanline
    cycle = 0;
    
    // Clear buffers
    for (int i = 0; i < 256 * 240; i++) {
        frame_buffer[i] = 0;
    }
    
    for (int i = 0; i < 0x2000; i++) {
        vram[i] = 0;
    }
    
    for (int i = 0; i < 0x20; i++) {
        palette_ram[i] = 0;
    }
    
    for (int i = 0; i < 256; i++) {
        spr_ram[i] = 0;
    }
}

// Register getters
unsigned char PPU::getPPUCTRL() {
    return nes_cpu->getMemoryDir(0x2000);
}

unsigned char PPU::getPPUMASK() {
    return nes_cpu->getMemoryDir(0x2001);
}

unsigned char PPU::getPPUSTATUS() {
    unsigned char status = nes_cpu->getMemoryDir(0x2002);
    // Reading PPUSTATUS clears the w latch
    w = false;
    // Also clears vblank flag (bit 7)
    status &= 0x7F;
    nes_cpu->setMemoryDir(0x2002, status);
    return status;
}

unsigned char PPU::getOAMADDR() {
    return nes_cpu->getMemoryDir(0x2003);
}

unsigned char PPU::getOAMDATA() {
    return spr_ram[getOAMADDR()];
}

unsigned char PPU::getPPUSCROLL() {
    return nes_cpu->getMemoryDir(0x2005);
}

unsigned char PPU::getPPUADDR() {
    return nes_cpu->getMemoryDir(0x2006);
}

unsigned char PPU::getPPUDATA() {
    unsigned char data = ppuRead(v);
    
    // Increment address based on PPUCTRL bit 2
    if (getPPUCTRL() & 0x04) {
        v += 32;  // Vertical increment
    } else {
        v += 1;   // Horizontal increment
    }
    
    return data;
}

// Register setters
void PPU::setPPUCTRL(unsigned char value) {
    nes_cpu->setMemoryDir(0x2000, value);
    // Update t register bits 10 and 11
    t = (t & 0xF3FF) | ((value & 0x03) << 10);
}

void PPU::setPPUMASK(unsigned char value) {
    nes_cpu->setMemoryDir(0x2001, value);
}

void PPU::setPPUSTATUS(unsigned char value) {
    nes_cpu->setMemoryDir(0x2002, value);
}

void PPU::setOAMADDR(unsigned char value) {
    nes_cpu->setMemoryDir(0x2003, value);
}

void PPU::setOAMDATA(unsigned char value) {
    unsigned char addr = getOAMADDR();
    spr_ram[addr] = value;
    // Auto-increment address
    nes_cpu->setMemoryDir(0x2003, addr + 1);
}

void PPU::setPPUSCROLL(unsigned char value) {
    nes_cpu->setMemoryDir(0x2005, value);
    
    if (!w) {
        // First write: horizontal scroll
        t = (t & 0xFFE0) | (value >> 3);
        x = value & 0x07;
    } else {
        // Second write: vertical scroll
        t = (t & 0x0C1F) | ((value & 0xF8) << 2);
        t = (t & 0xF3FF) | ((value & 0x07) << 12);
    }
    w = !w;
}

void PPU::setPPUADDR(unsigned char value) {
    nes_cpu->setMemoryDir(0x2006, value);
    
    if (!w) {
        // First write: high byte
        t = (t & 0x00FF) | ((value & 0x3F) << 8);
    } else {
        // Second write: low byte
        t = (t & 0xFF00) | value;
        v = t;  // Copy t to v
    }
    w = !w;
}

void PPU::setPPUDATA(unsigned char value) {
    ppuWrite(v, value);
    
    // Increment address based on PPUCTRL bit 2
    if (getPPUCTRL() & 0x04) {
        v += 32;  // Vertical increment
    } else {
        v += 1;   // Horizontal increment
    }
}

void PPU::setOAMDMA(unsigned char value) {
    // OAM DMA transfer
    unsigned short page = value << 8;
    
    for (int i = 0; i < 256; i++) {
        spr_ram[i] = nes_cpu->getMemoryDir(page + i);
    }
}

// Memory access functions
unsigned char PPU::ppuRead(unsigned short addr) {
    addr &= 0x3FFF;  // Mirror down to 16KB
    
    if (addr < 0x2000) {
        // Pattern tables
        if (nes_cartridge->getCHRROM() != nullptr) {
            return nes_cartridge->getCHRROM()[addr];
        } else {
            // CHR RAM
            return vram[addr];
        }
    } else if (addr < 0x3F00) {
        // Nametables
        unsigned short mirrored_addr = mirrorNametableAddr(addr);
        return vram[mirrored_addr - 0x2000];
    } else if (addr < 0x4000) {
        // Palette RAM
        addr &= 0x001F;
        // Handle mirroring
        if (addr == 0x0010 || addr == 0x0014 || addr == 0x0018 || addr == 0x001C) {
            addr -= 0x0010;
        }
        return palette_ram[addr];
    }
    return 0;
}

void PPU::ppuWrite(unsigned short addr, unsigned char value) {
    addr &= 0x3FFF;
    
    if (addr < 0x2000) {
        // Pattern tables (CHR RAM)
        vram[addr] = value;
    } else if (addr < 0x3F00) {
        // Nametables
        unsigned short mirrored_addr = mirrorNametableAddr(addr);
        vram[mirrored_addr - 0x2000] = value;
    } else if (addr < 0x4000) {
        // Palette RAM
        addr &= 0x001F;
        // Handle mirroring
        if (addr == 0x0010 || addr == 0x0014 || addr == 0x0018 || addr == 0x001C) {
            addr -= 0x0010;
        }
        palette_ram[addr] = value;
    }
}

// Mirroring helper
unsigned short PPU::mirrorNametableAddr(unsigned short addr) {
    unsigned short index = addr & 0x0FFF;
    unsigned short nametable = index / 0x0400;
    unsigned short offset = index % 0x0400;
    
    // Get mirroring mode from cartridge
    bool scrollMode = nes_cartridge->getScrollMode();
    
    if (scrollMode == 0) {
        // Horizontal mirroring
        if (nametable == 0 || nametable == 2) {
            return 0x2000 + offset;
        } else {
            return 0x2400 + offset;
        }
    } else {
        // Vertical mirroring
        if (nametable == 0 || nametable == 1) {
            return 0x2000 + offset;
        } else {
            return 0x2800 + offset;
        }
    }
}

// Rendering functions
void PPU::runCycle() {
    // This is the main PPU cycle function that needs to be called 3 times per CPU cycle
    
    // Pre-render scanline
    if (scanline == 261) {
        if (cycle == 1) {
            // Clear VBLANK flag and sprite 0 hit
            unsigned char status = getPPUSTATUS();
            status &= 0x5F;  // Clear bits 7 and 6
            setPPUSTATUS(status);
        }
    }
    
    // Visible scanlines (0-239)
    if (scanline >= 0 && scanline < 240) {
        renderPixel();
    }
    
    // Set VBLANK flag at start of VBLANK period
    if (scanline == 241 && cycle == 1) {
        unsigned char status = getPPUSTATUS();
        status |= 0x80;  // Set VBLANK flag
        setPPUSTATUS(status);
        
        // Trigger NMI if enabled in PPUCTRL
        if (getPPUCTRL() & 0x80) {
            // Need to trigger NMI on CPU - this requires CPU interface
        }
    }
    
    // Background fetching during visible and pre-render scanlines
    if (scanline < 240 || scanline == 261) {
        if ((cycle >= 1 && cycle <= 256) || (cycle >= 321 && cycle <= 336)) {
            fetchBackgroundData();
        }
        
        // Increment coarse X at cycle 256
        if (cycle == 256) {
            if ((v & 0x001F) == 31) {
                v &= ~0x001F;
                v ^= 0x0400;  // Switch horizontal nametable
            } else {
                v++;
            }
        }
        
        // Increment Y at cycle 257
        if (cycle == 257) {
            evaluateSprites();
            
            // Copy horizontal bits from t to v
            v = (v & 0xFBE0) | (t & 0x041F);
        }
        
        // Handle fine Y increment
        if (cycle == 304 && scanline == 261) {
            // Copy vertical bits from t to v
            v = (v & 0x041F) | (t & 0xFBE0);
        }
    }
    
    // Increment cycle/scanline
    cycle++;
    if (cycle > 340) {
        cycle = 0;
        scanline++;
        if (scanline > 261) {
            scanline = 0;
            odd_frame = !odd_frame;
        }
    }
}

void PPU::renderPixel() {
    if (cycle < 1 || cycle > 256) return;
    
    int pixel_x = cycle - 1;
    int pixel_y = scanline;
    int pixel_index = pixel_y * 256 + pixel_x;
    
    unsigned int bg_color = 0;
    unsigned int spr_color = 0;
    bool bg_opaque = false;
    bool spr_opaque = false;
    bool bg_priority = true;
    
    // Background rendering
    if (getPPUMASK() & 0x08) {
        unsigned int shift = 15 - x;
        unsigned char bg_pixel = ((bg_pattern_low >> shift) & 0x01) |
                                 (((bg_pattern_high >> shift) & 0x01) << 1);
        
        if (bg_pixel != 0) {
            unsigned char palette = ((bg_palette_low >> shift) & 0x01) |
                                    (((bg_palette_high >> shift) & 0x01) << 1);
            bg_color = readPalette(palette * 4 + bg_pixel);
            bg_opaque = true;
        }
    }
    
    // Sprite rendering
    if (getPPUMASK() & 0x10) {
        for (int i = 0; i < sprite_count; i++) {
            int spr_x = pixel_x - sprite_positions[i];
            if (spr_x >= 0 && spr_x < 8) {
                // Get sprite pixel (simplified)
                unsigned char spr_pixel = 1;  // Placeholder
                
                if (spr_pixel != 0) {
                    spr_color = readPalette(0x10 + (sprite_attributes[i] & 0x03) * 4 + spr_pixel);
                    spr_opaque = true;
                    bg_priority = !(sprite_attributes[i] & 0x20);
                    break;
                }
            }
        }
    }
    
    // Combine pixels
    unsigned int final_color = 0;
    
    if (!spr_opaque && !bg_opaque) {
        final_color = readPalette(0);  // Background color
    } else if (!spr_opaque) {
        final_color = bg_color;
    } else if (!bg_opaque) {
        final_color = spr_color;
    } else {
        if (bg_priority) {
            final_color = bg_color;
        } else {
            final_color = spr_color;
        }
    }
    
    frame_buffer[pixel_index] = final_color;
}

void PPU::fetchBackgroundData() {
    unsigned char phase = (cycle - 1) % 8;
    
    switch (phase) {
        case 0:
            // Fetch nametable byte
            nt_byte = ppuRead(0x2000 | (v & 0x0FFF));
            break;
            
        case 2:
            // Fetch attribute byte
            {
                unsigned short attr_addr = 0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
                unsigned char attr = ppuRead(attr_addr);
                unsigned char shift = ((v >> 4) & 4) | (v & 2);
                at_byte = (attr >> shift) & 0x03;
            }
            break;
            
        case 4:
            // Fetch pattern table low byte
            {
                unsigned short pattern_addr = (getPPUCTRL() & 0x10) << 8;
                pattern_addr |= nt_byte << 4;
                pattern_addr |= (v >> 12) & 0x07;
                bg_low = ppuRead(pattern_addr);
            }
            break;
            
        case 6:
            // Fetch pattern table high byte
            {
                unsigned short pattern_addr = (getPPUCTRL() & 0x10) << 8;
                pattern_addr |= nt_byte << 4;
                pattern_addr |= (v >> 12) & 0x07;
                pattern_addr += 8;
                bg_high = ppuRead(pattern_addr);
            }
            break;
    }
    
    // Shift registers on appropriate phases
    if (phase == 1) {
        // Shift background registers
        bg_pattern_low <<= 1;
        bg_pattern_high <<= 1;
        bg_palette_low <<= 1;
        bg_palette_high <<= 1;
    }
}

void PPU::evaluateSprites() {
    sprite_count = 0;
    unsigned char sprite_height = (getPPUCTRL() & 0x20) ? 16 : 8;
    
    for (int i = 0; i < 64 && sprite_count < 8; i++) {
        unsigned char y = spr_ram[i * 4];
        unsigned char x = spr_ram[i * 4 + 3];
        unsigned char tile = spr_ram[i * 4 + 1];
        unsigned char attr = spr_ram[i * 4 + 2];
        
        // Check if sprite is on current scanline
        int diff = scanline - y;
        if (diff >= 0 && diff < sprite_height) {
            sprite_positions[sprite_count] = x;
            sprite_patterns[sprite_count] = tile;
            sprite_attributes[sprite_count] = attr;
            sprite_priorities[sprite_count] = (attr & 0x20) ? 1 : 0;
            sprite_count++;
        }
    }
}

unsigned int PPU::readPalette(unsigned char index) {
    unsigned char color_index = ppuRead(0x3F00 + (index & 0x1F));
    
    // Standard NES palette (RGB format)
    static const unsigned int nes_palette[64] = {
        0x666666, 0x002A88, 0x1412A7, 0x3B00A4, 0x5C007E, 0x6E0040, 0x6C0600, 0x561D00,
        0x333500, 0x0B4800, 0x005200, 0x004F08, 0x00404D, 0x000000, 0x000000, 0x000000,
        0xADADAD, 0x155FD9, 0x4240FF, 0x7527FE, 0xA01ACC, 0xB71E7B, 0xB53120, 0x994E00,
        0x6B6D00, 0x388700, 0x0C9300, 0x008F32, 0x007C8D, 0x000000, 0x000000, 0x000000,
        0xFFFEFF, 0x64B0FF, 0x9290FF, 0xC676FF, 0xF36AFF, 0xFE6ECC, 0xFE8170, 0xEA9E22,
        0xBCBE00, 0x88D800, 0x5CE430, 0x45E082, 0x48CDDE, 0x4F4F4F, 0x000000, 0x000000,
        0xFFFEFF, 0xC0DFFF, 0xD3D2FF, 0xE8C8FF, 0xFBC2FF, 0xFEC4EA, 0xFECCC5, 0xF7D8A5,
        0xE4E594, 0xCFEF96, 0xBDF4AB, 0xB3F3CC, 0xB5EBF2, 0xB8B8B8, 0x000000, 0x000000
    };
    
    return nes_palette[color_index & 0x3F];
}

// Frame control
void PPU::startFrame() {
    scanline = 261;
    cycle = 0;
}

void PPU::endFrame() {
    // Nothing to do here for now
}

bool PPU::isFrameReady() {
    return scanline == 241 && cycle == 1;
}

unsigned int* PPU::getFrameBuffer() {
    return frame_buffer;
}

// Status helpers
bool PPU::isRenderingEnabled() {
    return (getPPUMASK() & 0x18) != 0;
}

bool PPU::isSpriteZeroHit() {
    return (getPPUSTATUS() & 0x40) != 0;
}

void PPU::setSpriteZeroHit(bool hit) {
    unsigned char status = getPPUSTATUS();
    if (hit) {
        status |= 0x40;
    } else {
        status &= ~0x40;
    }
    setPPUSTATUS(status);
}

// Debug functions
void PPU::dumpVRAM() {
    printf("VRAM Dump:\n");
    for (int i = 0; i < 0x2000; i += 16) {
        printf("%04X: ", i);
        for (int j = 0; j < 16; j++) {
            printf("%02X ", vram[i + j]);
        }
        printf("\n");
    }
}

void PPU::dumpPalette() {
    printf("Palette RAM Dump:\n");
    for (int i = 0; i < 0x20; i += 16) {
        printf("%02X: ", i);
        for (int j = 0; j < 16; j++) {
            printf("%02X ", palette_ram[i + j]);
        }
        printf("\n");
    }
}