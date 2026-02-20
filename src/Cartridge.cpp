#include <stdio.h>
#include "../include/Cartridge.hpp"

Cartridge :: Cartridge(){
    m_CHR_ROM = nullptr;
    m_PRG_ROM = nullptr;
    m_Trainer = nullptr;
    prg_rom_size = 0;
    chr_rom_size = 0;
    scrollMode = false;
    mapper = -1;
    thereIsPRGRAM = false;
    thereIsCHRRAM = false;
    prg_ram_size = 0;
    NTSC_or_PAL = 0;
    selectedPRGBank = 0;
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

                if (!NES2_0MODE){
                    
                    printf("Modo iNES\n");

                    fread(&prg_ram_size, sizeof(unsigned char), 1, romFile);
                    fread(&NTSC_or_PAL, sizeof(unsigned char), 1, romFile);

                    fread(&otherFlags1, sizeof(short), 1, romFile);
                    fread(&otherFlags2, sizeof(int), 1, romFile);
                    
                    // Read trainer if present (512 bytes)
                    if (thereIsTrainer) {
                        //fseek(romFile, 512, SEEK_CUR); <---- Skip trainer
                        fread(m_Trainer, sizeof(unsigned char), 512, romFile);
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
                }else{

                    printf("Modo NES 2.0\n");

                    unsigned char mapper_submapper;

                    fread(&mapper_submapper, sizeof(unsigned char), 1, romFile);

                    unsigned char aux;

                    fread(&aux, sizeof(unsigned char), 1, romFile);

                    prg_rom_size |= (aux & 0x0F) << 8;
                    chr_rom_size |= (aux & 0xF0) << 4;

                    fread(&aux, sizeof(unsigned char), 1, romFile);

                    aux = aux & 0x0F;
                    prg_ram_size = 64 << aux;

                    fread(&aux, sizeof(unsigned char), 1, romFile);

                    aux = aux & 0x0F;
                    chr_ram_size = 64 << aux;

                    fread(&otherFlags1, sizeof(int), 1, romFile);

                    // Read trainer if present (512 bytes)
                    if (thereIsTrainer) {
                        //fseek(romFile, 512, SEEK_CUR); <---- Skip trainer
                        fread(m_Trainer, sizeof(unsigned char), 512, romFile);
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
        }

        printf("Cartridge loaded successfully\n");

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

void Cartridge :: catchWriteInRAM(unsigned short dir, unsigned char value){
    /*
    if (dir < 0xC000) 
        m_PRG_ROM[selectedPRGBank * 16384 + (dir % 16384)] = value;
    else
        m_PRG_ROM[(prg_rom_size - 1) * 16384 + (dir % 16384)] = value;
    */
    //selectedPRGBank = value & 0x0F;

    if (prg_rom_size == 1){
        m_PRG_ROM[dir % 16384] = value;
    }else if (prg_rom_size == 2){
        m_PRG_ROM[dir % 32768] = value;
    }else{
        if (dir < 0xC000) {
            //printf("Direccion: %x\n", selectedPRGBank * 16384 + (dir % 16384));
            //printf("VALOR: %x\n", m_PRG_ROM[selectedPRGBank * 16384 + (dir % 16384)]);
            m_PRG_ROM[selectedPRGBank * 16384 + (dir % 16384)] = value;
        }else{
            //printf("Direccion: %x\n", (prg_rom_size - 1) * 16384 + (dir % 16384));
            //printf("VALOR: %x\n", m_PRG_ROM[(prg_rom_size - 1) * 16384 + (dir % 16384)]);
            m_PRG_ROM[(prg_rom_size - 1) * 16384 + (dir % 16384)] = value;
        }
    }

    selectedPRGBank = value & 0x07;
    printf("SelectedPRGBank: %x\n", selectedPRGBank);
}

unsigned char Cartridge :: getPRGValue(unsigned short dir){
/*
    if (dir < 0xC000) {
        printf("Direccion: %x\n", selectedPRGBank * 16384 + (dir % 16384));
        printf("VALOR: %x\n", m_PRG_ROM[selectedPRGBank * 16384 + (dir % 16384)]);
        return m_PRG_ROM[selectedPRGBank * 16384 + (dir % 16384)];
    }else{
        printf("Direccion: %x\n", (prg_rom_size - 1) * 16384 + (dir % 16384));
        printf("VALOR: %x\n", m_PRG_ROM[(prg_rom_size - 1) * 16384 + (dir % 16384)]);
        return m_PRG_ROM[(prg_rom_size - 1) * 16384 + (dir % 16384)];
    }
*/
    if (prg_rom_size == 1){
        return m_PRG_ROM[dir % 16384];
    }else if (prg_rom_size == 2){
        return m_PRG_ROM[dir % 32768];
    }else{
        if (dir < 0xC000) {
            //printf("Direccion: %x\n", selectedPRGBank * 16384 + (dir % 16384));
            //printf("VALOR: %x\n", m_PRG_ROM[selectedPRGBank * 16384 + (dir % 16384)]);
            return m_PRG_ROM[selectedPRGBank * 16384 + (dir % 16384)];
        }else{
            //printf("Direccion: %x\n", (prg_rom_size - 1) * 16384 + (dir % 16384));
            //printf("VALOR: %x\n", m_PRG_ROM[(prg_rom_size - 1) * 16384 + (dir % 16384)]);
            return m_PRG_ROM[(prg_rom_size - 1) * 16384 + (dir % 16384)];
        }
    }
}

unsigned char * Cartridge :: getPRGDir(unsigned short dir){
    
    if (prg_rom_size == 1){
        return &m_PRG_ROM[dir % 16384];
    }else if (prg_rom_size == 2){
        return &m_PRG_ROM[dir % 32768];
    }else{
        if (dir < 0xC000) 
            return &m_PRG_ROM[selectedPRGBank * 16384 + (dir % 16384)];
        else
            return &m_PRG_ROM[(prg_rom_size - 1) * 16384 + (dir % 16384)];        
    }

}

Cartridge :: ~Cartridge(){
    
    if (m_CHR_ROM != NULL) 
        delete [] m_CHR_ROM;

    if (m_PRG_ROM != NULL) 
        delete [] m_PRG_ROM;
}