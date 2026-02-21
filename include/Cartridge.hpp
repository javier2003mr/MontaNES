#ifndef CARTRIDGE
#define CARTRIDGE

class Cartridge {
    private:
        unsigned char * m_PRG_ROM;
        unsigned char * m_CHR_ROM;
        unsigned char * m_Trainer;
        unsigned short prg_rom_size;
        unsigned short chr_rom_size;
        bool scrollMode;
        unsigned char mapper;
        bool thereIsPRGRAM;
        bool thereIsCHRRAM;
        unsigned char prg_ram_size;
        unsigned char chr_ram_size;
        unsigned char NTSC_or_PAL;
        unsigned char selectedPRGBank;
        unsigned char selectedCHRBank;
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
        void catchWriteInRAM(unsigned short dir, unsigned char value);
        void catchWriteInVRAM(unsigned short dir, unsigned char value);
        unsigned char getPRGValue(unsigned short dir);
        unsigned char getCHRValue(unsigned short dir);
        unsigned char * getPRGDir(unsigned short dir);
        ~Cartridge();
};

#endif