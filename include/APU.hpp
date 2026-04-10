#ifndef APU_CLASS
#define APU_CLASS

#include <SoundPlayer.h>

#define TAM_APU_REGISTERS 4

const int32 SAMPLE_RATE = 44100;
const size_t BUFFER_SIZE = 125 * sizeof(float);
const float AMPLITUDE = 1.0f;
const float FREQUENCY = 440.0f; // Nota LA (A)

static const unsigned char DUTY_CYCLE_SEQUENCES[4][8] = {
    {0, 1, 0, 0, 0, 0, 0, 0}, // 00: 12.5%
    {0, 1, 1, 0, 0, 0, 0, 0}, // 01: 25%
    {0, 1, 1, 1, 1, 0, 0, 0}, // 10: 50%
    {1, 0, 0, 1, 1, 1, 1, 1}  // 11: 75%
};

static const unsigned char TRIANGLE_SEQUENCE[32] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0
};

static const unsigned short NOISE_PERIODS[16] = {
    4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
};

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

        // New member variables for Pulse Wave state
        unsigned short pulse_timer_counter[2];
        unsigned char pulse_current_sequence_step[2];
        unsigned char pulse_envelope_volume[2];
        unsigned char pulse_length_counter[2];
        unsigned char pulse_sweep_counter[2];
        float pulse_output[2];

        // Private helper function for generating pulse samples
        float generatePulseSample(int pulse_index);

        // New member variables for Triangular Wave state
        unsigned short triangle_timer_counter;
        unsigned char triangle_linear_counter;
        unsigned char triangle_length_counter;
        unsigned char triangle_current_sequence_step;
        float triangle_output;

        // Private helper function for generating triangle samples
        float generateTriangleSample();

        // New member variables for Noise Channel state
        unsigned short noise_timer_counter;
        unsigned char noise_envelope_volume;
        unsigned char noise_length_counter;
        unsigned short noise_shift_register; // 15-bit LFSR
        float noise_output;

        // Private helper function for generating noise samples
        float generateNoiseSample();

    public:

        APU();
        ~APU();

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

        //Noise wave
        bool getNoiseEnvelopeLoop();
        bool getNoiseConstantVolume();
        unsigned char getNoiseVolumeEnvelope();
        bool getNoiseMode();
        unsigned char getNoisePeriod();
        unsigned char getNoiseLengthCounterLoad();

        //Triangular wave
        bool getTriangleLengthCounterHalt();
        unsigned char getTriangleLinearCounterLoad();
        unsigned short getTriangleTimer();
        unsigned char getTriangleLengthCounterLoad();

        void start();
        void stop();
        bool isPlaying();
};

#endif