#ifndef CARTRIDGE
#define CARTRIDGE

class Cartridge {
    private:
        unsigned char * m_PRG_ROM;
        unsigned char * m_CHR_ROM;
        unsigned char prg_rom_size;
        unsigned char chr_rom_size;
        bool scrollMode;
        unsigned char mapper;
        bool thereIsPRGRAM;
        bool thereIsCHRRAM;
    public:
        Cartridge();
        void loadROM (char * path);
        bool getScrollMode();
        unsigned char * getPRGROM();
        unsigned char getPRGROMSize();
        unsigned char * getCHRROM();
        unsigned char getCHRROMSize();
        unsigned char getMapper();
        bool hasExtendedRAM();
        ~Cartridge();
};

#endif