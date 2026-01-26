#ifndef PPUCLASS
#define PPUCLASS

#include "CPU.h"

class PPU {

    private:

        CPU * nes_cpu;

        // Current VRAM address (lowests 15 bits)
        unsigned short v;

        // Temporary VRAM address (lowests 15 bits)
        // Top left onscreen title address
        unsigned short t;

        // Fine X scroll (3 bits)
        unsigned char x;

        // First or second write toggle
        bool w;

    public:

        unsigned char getPPUCTRL();
        unsigned char getPPUMASK();
        unsigned char getPPUSTATUS();
        unsigned char getOAMADDR();
        unsigned char getPPUSCROLL();
        unsigned char getPPUADDR();
        unsigned char getPPUDATA();
        unsigned char getOAMDMA();

        void setPPUCTRL (unsigned char value);
        void setPPUMASK (unsigned char value);
        void setPPUSTATUS (unsigned char value);
        void setOAMADDR (unsigned char value);
        void setPPUSCROLL (unsigned char value);
        void setPPUADDR (unsigned char value);
        void setPPUDATA (unsigned char value);
        void setOAMDMA (unsigned char value);

};

#endif