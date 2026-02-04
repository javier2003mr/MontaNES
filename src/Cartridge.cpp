#include <stdio.h>
#include "../include/Cartridge.hpp"

Cartridge :: Cartridge(){
    m_CHR_ROM = nullptr;
    m_PRG_ROM = nullptr;
    prg_rom_size = 0;
    chr_rom_size = 0;
    scrollMode = false;
    mapper = -1;
    thereIsPRGRAM = false;
    thereIsCHRRAM = false;
}

void Cartridge :: loadROM (char * path){
    
    FILE * romFile;
    romFile = fopen(path, "rb");

    if (romFile == NULL){
        printf("Error when opening the file: Not found\n");
    }else{

        int magic_number;

        fread(&magic_number, sizeof(int), 1, romFile);

        if (magic_number != 0x1A53454E){
            printf("Error when reading the file: It is not a .nes file\n");
        }else{

            fread(&prg_rom_size, sizeof(unsigned char), 1, romFile);

            if (!prg_rom_size){
                printf("Error: No PRG code\n");
            }else{
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

                // Skip trainer if present (512 bytes)
                if (thereIsTrainer) {
                    fseek(romFile, 512, SEEK_CUR);
                }

                // Load PRG-ROM
                m_PRG_ROM = new unsigned char[prg_rom_size * 16384];
                fread(m_PRG_ROM, sizeof(unsigned char), prg_rom_size * 16384, romFile);

                // Load CHR-ROM or allocate CHR-RAM
                if (chr_rom_size > 0) {
                    m_CHR_ROM = new unsigned char[chr_rom_size * 8192];
                    fread(m_CHR_ROM, sizeof(unsigned char), chr_rom_size * 8192, romFile);
                } else {
                    // If no CHR-ROM, cartridge uses CHR-RAM (8KB)
                    m_CHR_ROM = new unsigned char[8192]; // Allocate 8KB for CHR-RAM
                    // Initialize CHR-RAM to 0
                    for (int i = 0; i < 8192; ++i) {
                        m_CHR_ROM[i] = 0;
                    }
                    thereIsCHRRAM = true;
                }
            }
        }
        fclose(romFile);
    }
}

bool Cartridge :: getScrollMode(){
    return scrollMode;
}

unsigned char * Cartridge :: getPRGROM() {
    return m_PRG_ROM;
}

unsigned char Cartridge :: getPRGROMSize() {
    return prg_rom_size;
}

unsigned char * Cartridge :: getCHRROM() {
    return m_CHR_ROM;
}

unsigned char Cartridge :: getCHRROMSize() {
    return chr_rom_size;
}

unsigned char Cartridge :: getMapper(){
    return mapper;
}

bool Cartridge :: hasExtendedRAM(){
    return true;
}

Cartridge :: ~Cartridge(){
    
    if (m_CHR_ROM != NULL) 
        delete [] m_CHR_ROM;

    if (m_PRG_ROM != NULL) 
        delete [] m_PRG_ROM;
}