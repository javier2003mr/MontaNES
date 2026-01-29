#include <stdio.h>
#include <iostream>
#include <fstream>
#include "../../include/Cartridge.hpp"
/*
int main (int argc, char ** argv){

    if (argc != 2){
        printf("El número de atributos no es correcto: test_load_patterns <rom.nes>\n");
        return 1;
    }

    std::ifstream file;

    file.open(argv[1], std::ifstream::in);

    if (file.is_open()){

        char c;
        char prg_rom_size, chr_rom_size;
        uint8_t mapper;
        bool thereIsTrainer;
        bool thereIsPRGRAM;
        bool scrollMode;
        bool NES2_0MODE;

        printf("Número mágico: ");

        // Magic Number
        for (int i = 0; i < 4; ++i){
            file.get(c);
            printf("0x%x ", c);
        }

        printf("\n");

        file.get(prg_rom_size);
        file.get(chr_rom_size);

        printf("Número de bloques de 16KB (PRG ROM): 0x%x\n", prg_rom_size);
        printf("Número de bloques de 8KB (CHR ROM): 0x%x\n", chr_rom_size);

        printf("Flags:\n");
        file.get(c);
        printf("0x%x ", c);

        mapper = c >> 4;
        thereIsTrainer = c & 0x04;
        thereIsPRGRAM = c & 0x02;
        scrollMode = c & 0x01;

        file.get(c);
        printf("0x%x \n", c);

        mapper |= c & 0xF0;
        NES2_0MODE = ((c & 0x0C) == 0x8);

        if (mapper == 0){}

        uint8_t prg_rom[prg_rom_size * 16384];
        uint8_t chr_rom[chr_rom_size * 8192];           

        printf("Resto de la cabecera:\n");
        for (int i = 0; i < 8; ++i){
            file.get(c);
            printf("0x%x\t", c);
        }
        printf("\n\n");
        
        for (int i = 0; i < prg_rom_size * 16384; ++i){
            file.get(c);
            prg_rom[i] = c;
        }
        
        for (int i = 0; i < chr_rom_size * 8192; ++i){
            file.get(c);
            chr_rom[i] = c;
        }
            
        file.get(c);
        printf("Reset: 0x%x", c);
        file.get(c);
        printf("%x\n\n", c);

        printf("PRG ROM:\n");
        for (int i = 0; i < prg_rom_size * 16384; ++i){
            
            if (i % 0xF == 0){
                printf("\n");
            }
            
            printf("%x\t", prg_rom[i]);
        }

        printf("\n\nCHR ROM:\n");
        for (int i = 0; i < chr_rom_size * 8192; ++i){
            
            if (i % 0xF == 0){
                printf("\n");
            }

            printf("%x\t", chr_rom[i]);
        }

    }

}
*/

int main (int argc, char ** argv){
    
    if (argc != 2){
        printf("Error: Número erroneo de argumentos: ./test_load_patterns <rom.nes>\n");
        return 1;
    }

    Cartridge cartucho;

    cartucho.loadROM(argv[1]);

    return 0;

}