#include "../include/PPU.hpp"

unsigned char PPU :: getPPUCTRL(){
    return nes_cpu->getMemoryDir(0x2000);
}

unsigned char PPU :: getPPUMASK(){
    return nes_cpu->getMemoryDir(0x2001);
}

unsigned char PPU :: getPPUSTATUS(){
    return nes_cpu->getMemoryDir(0x2002);
}

unsigned char PPU :: getOAMADDR(){
    return nes_cpu->getMemoryDir(0x2003);
}

unsigned char PPU :: getPPUSCROLL(){
    return nes_cpu->getMemoryDir(0x2004);
}

unsigned char PPU :: getPPUADDR(){
    return nes_cpu->getMemoryDir(0x2005);
}

unsigned char PPU :: getPPUDATA(){
    return nes_cpu->getMemoryDir(0x2006);
}

unsigned char PPU :: getOAMDMA(){
    return nes_cpu->getMemoryDir(0x2007);
}

void PPU :: setPPUCTRL (unsigned char value){
    nes_cpu->setMemoryDir(0x2000, value);
}

void PPU :: setPPUMASK (unsigned char value){
    nes_cpu->setMemoryDir(0x2001, value);
}

void PPU :: setPPUSTATUS (unsigned char value){
    nes_cpu->setMemoryDir(0x2002, value);
}

void PPU :: setOAMADDR (unsigned char value){
    nes_cpu->setMemoryDir(0x2003, value);
}

void PPU :: setPPUSCROLL (unsigned char value){
    nes_cpu->setMemoryDir(0x2004, value);
}

void PPU :: setPPUADDR (unsigned char value){
    nes_cpu->setMemoryDir(0x2005, value);
}

void PPU :: setPPUDATA (unsigned char value){
    nes_cpu->setMemoryDir(0x2006, value);
}

void PPU :: setOAMDMA (unsigned char value){
    nes_cpu->setMemoryDir(0x2007, value);
}