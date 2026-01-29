#ifndef CARTRIDGE
#define CARTRIDGE

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
        bool getScrollMode();
        unsigned char * getPRGROM();
        unsigned char * getCHRROM();
        unsigned char getMapper();
        bool hasExtendedRAM();
        ~Cartridge();
};

#endif