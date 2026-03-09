#ifndef APU_CLASS
#define APU_CLASS

#define TAM_APU_REGISTERS 4

class APU {

    private:

        // AUDIO WAVES REGISTERS
        unsigned char regPulse1 [TAM_APU_REGISTERS];
        unsigned char regPulse2 [TAM_APU_REGISTERS];
        unsigned char regTriangle [TAM_APU_REGISTERS];
        unsigned char regNoise [TAM_APU_REGISTERS];
        unsigned char regDMC [TAM_APU_REGISTERS];
        
        // Channel enable and length counter status register
        // ---D NT21
        // Most common use: 0000 1111 (All in use but the DMC)
        unsigned char reg4015;
        
        // Frame Counter register
        unsigned char reg4017;

    public:

        void reset();
        unsigned char getAudioValue(unsigned short dir);
        unsigned char * getAudioDir(unsigned short dir);
        void setAudioValue(unsigned short dir, unsigned char value);
};

#endif