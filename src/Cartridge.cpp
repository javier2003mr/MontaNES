#include <stdio.h>
#include "../include/Cartridge.hpp"

Cartridge :: Cartridge(){
    m_CHR_ROM = nullptr;
    m_CHR_ROM = nullptr;
    scrollMode = false;
    mapper = -1;
    thereIsPRGRAM = false;
    thereIsCHRRAM = false;
}

void Cartridge :: loadROM (char * path){
    
    FILE * romFile;
    romFile = fopen(path, "rb");

    if (romFile == nullptr){
        perror("Error when opening the file: Not found\n");
    }else{

        int magic_number;

        fread(&magic_number, sizeof(int), 1, romFile);

        if (magic_number != 0x4E45531A){
            perror("Error when reading the file: It is not a .nes file\n");
        }else{

            unsigned char prg_rom_size;

            fread(&prg_rom_size, sizeof(unsigned char), 1, romFile);

            if (!prg_rom_size){
                perror("Error: No PRG code\n");
            }else{

                unsigned char chr_rom_size;
                unsigned char flags6, flags7;
                bool thereIsTrainer, NES2_0MODE;
                int otherFlags1, otherFlags2;

                fread(&chr_rom_size, sizeof(unsigned char), 1, romFile);

                fread(&flags6, sizeof(unsigned char), 1, romFile);

                mapper = flags6 >> 4;
                thereIsTrainer = flags6 & 0x04;
                thereIsPRGRAM = flags6 & 0x02;
                scrollMode = flags6 & 0x01;
                
                fread(&flags7, sizeof(unsigned char), 1, romFile);

                mapper |= flags7 & 0xF0;
                NES2_0MODE = ((flags7 & 0x0C) == 0x8);

                if (mapper == 0){}

                fread(&otherFlags1, sizeof(int), 1, romFile);
                fread(&otherFlags2, sizeof(int), 1, romFile);

                m_PRG_ROM = new unsigned char [prg_rom_size * 16384];
                fread(m_PRG_ROM, sizeof(unsigned char), prg_rom_size * 16384, romFile);
                
                if (chr_rom_size){
                    m_CHR_ROM = new unsigned char [chr_rom_size * 8192];
                    fread(m_CHR_ROM, sizeof(unsigned char), chr_rom_size * 8192, romFile);
                }else{
                    printf("This ROM uses CHR RAM, not CHR ROM\n");
                }
            }

        }
    }
}

bool Cartridge :: getScrollMode(){
    return scrollMode;
}

unsigned char * Cartridge :: getPRGROM() {
    return m_PRG_ROM;
}

unsigned char * Cartridge :: getCHRROM() {
    return m_CHR_ROM;
}

unsigned char Cartridge :: getMapper(){
    return mapper;
}

bool Cartridge :: hasExtendedRAM(){
    return true;
}

Cartridge :: ~Cartridge(){
    delete [] m_CHR_ROM;
    delete [] m_PRG_ROM;
}