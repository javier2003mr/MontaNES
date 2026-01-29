#include <stdio.h>
#include <iostream>
#include <fstream>

class Cartridge {
    private:
        unsigned char * m_PRG_ROM;
        unsigned char * m_CHR_ROM;
        bool scrollMode;
        unsigned char mapper;
        bool thereIsPRGRAM;
        bool thereIsCHRRAM;
    public:
        Cartridge();
        void loadROM (char * path);
        ~Cartridge();
};

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

Cartridge :: ~Cartridge(){
    delete [] m_CHR_ROM;
    delete [] m_PRG_ROM;
}

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