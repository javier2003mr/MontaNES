#ifndef APU_CLASS
#define APU_CLASS

#include <SoundPlayer.h>

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

        static void AudioCallback(void* cookie, void* buffer, size_t size, const media_raw_audio_format& format);
        BSoundPlayer* player;
        bool is_playing;

    public:

        APU();

        void reset();
        unsigned char getAudioValue(unsigned short dir);
        unsigned char * getAudioDir(unsigned short dir);
        void setAudioValue(unsigned short dir, unsigned char value);
        
        //Pulse wave
        double getDutyCycle(int pulse_index);
        double getVolume (int pulse_index);
        unsigned short getPulseTimer (int pulse_index);
        unsigned char getLengthCounter(int pulse_index);
        bool getLengthCounterHalt(int pulse_index);
        bool getEnvelopeFlag(int pulse_index);
        bool getSweepEnableFlag(int pulse_index);
        unsigned char getSweepDividersPeriod(int pulse_index);
        bool getSweepNegativeFlag(int pulse_index);
        unsigned char getSweepShiftCount(int pulse_index);

        void start();
        void stop();
        bool isPlaying();
};

#endif