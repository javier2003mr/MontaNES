#include "APU.hpp"
#include <stdio.h>

APU :: APU(){
    player = nullptr;
}

APU :: ~APU(){
    if (player){
        player->Stop();
        delete player;
        player = nullptr;
    }
}

void APU :: reset(){
    
    // Inicialización de los registros
    for (int i = 0; i < TAM_APU_REGISTERS; ++i){
        regPulse1[i] = 0x00;
        regPulse2[i] = 0x00;
        regTriangle[i] = 0x00;
        regNoise[i] = 0x00;
        regDMC[i] = 0x00;
    }

    reg4015 = 0x00;
    reg4017 = 0x00;

    regPulse1[0] = regPulse2[0] = regNoise[0] = 0x30;
    regPulse1[1] = regPulse2[1] = 0x08;
    regTriangle[0] = 0x80;

    // Initialize new member variables
    for (int i = 0; i < 2; ++i) {
        pulse_timer_counter[i] = 0;
        pulse_current_sequence_step[i] = 0;
        pulse_envelope_volume[i] = 0;
        pulse_length_counter[i] = 0;
        pulse_sweep_counter[i] = 0;
        pulse_output[i] = 0.0f;
    }

    // Initialize new member variables for Triangular Wave
    triangle_timer_counter = 0;
    triangle_linear_counter = 0;
    triangle_length_counter = 0;
    triangle_current_sequence_step = 0;
    triangle_output = 0.0f;

    // Initialize new member variables for Noise Channel
    noise_timer_counter = 0;
    noise_envelope_volume = 0;
    noise_length_counter = 0;
    noise_shift_register = 1; // Initial value for LFSR
    noise_output = 0.0f;

    //Parte del codigo de BSoundPlayer
    is_playing = false;
    
    media_raw_audio_format format;
    
    format.frame_rate = SAMPLE_RATE;
    format.channel_count = 1; // Mono
    format.format = media_raw_audio_format::B_AUDIO_FLOAT;
    format.buffer_size = BUFFER_SIZE; // 10ms buffer
    format.byte_order = B_MEDIA_LITTLE_ENDIAN;

    if (player != nullptr){
        delete player;
    }
    
    player = new BSoundPlayer (&format, "MontaNES Audio", &APU::AudioCallback, NULL, this);

    if (player->InitCheck() != B_OK) {
        printf("La inicialización de la APU ha fallado: %s\n", strerror(player->InitCheck()));
        delete player;
        player = nullptr;
    } else {
        player->SetVolume(AMPLITUDE);
        printf("APU inicializada con exito\n");
    }
}

unsigned char APU :: getAudioValue(unsigned short dir){

    unsigned char temp = 0;

    dir = dir - 0x4000;

    if (dir < 0x04){
        temp = regPulse1[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x08){
        temp = regPulse2[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x0C){
        temp = regTriangle[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x10){
        temp = regNoise[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x15){
        temp = regDMC[dir % TAM_APU_REGISTERS];
    }else if (dir == 0x15){
        temp = reg4015;
    }else if (dir == 0x17){
        temp = reg4017;
    }

    return temp;

}

unsigned char * APU :: getAudioDir(unsigned short dir){

    unsigned char *temp = nullptr;

    dir = dir - 0x4000;

    if (dir < 0x04){
        temp = &regPulse1[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x08){
        temp = &regPulse2[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x0C){
        temp = &regTriangle[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x10){
        temp = &regNoise[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x15){
        temp = &regDMC[dir % TAM_APU_REGISTERS];
    }else if (dir == 0x15){
        temp = &reg4015;
    }else if (dir == 0x17){
        temp = &reg4017;
    }

    return temp;

}

void APU :: setAudioValue(unsigned short dir, unsigned char value){

    dir = dir - 0x4000;

    if (dir < 0x04){
        regPulse1[dir % TAM_APU_REGISTERS] = value;
    }else if (dir < 0x08){
        regPulse2[dir % TAM_APU_REGISTERS] = value;
    }else if (dir < 0x0C){
        regTriangle[dir % TAM_APU_REGISTERS] = value;
    }else if (dir < 0x10){
        regNoise[dir % TAM_APU_REGISTERS] = value;
    }else if (dir < 0x15){
        regDMC[dir % TAM_APU_REGISTERS] = value;
    }else if (dir == 0x15){
        reg4015 = value;
    }else if (dir == 0x17){
        reg4017 = value;
    }

}

// Pulse Wave
double APU :: getDutyCycle(int pulse_index){
    
    unsigned char duty_cycle_raw;
    double duty_cycle;

    duty_cycle_raw = (!pulse_index) ? (regPulse1[0] & 0xC0) >> 6 : (regPulse2[0] & 0xC0) >> 6;

    switch (duty_cycle_raw)
    {
    case 0b00:
        duty_cycle = 0.125;
        break;
    case 0b01:
        duty_cycle = 0.25;
        break;
    case 0b10:
        duty_cycle = 0.50;
        break;
    case 0b11:
        duty_cycle = 0.75;
        break;
    default:
        duty_cycle = 0;
        break;
    }

    return duty_cycle;

}

unsigned char APU :: getLengthCounter(int pulse_index){
    return (!pulse_index) ? ((regPulse1[3] & 0xF8) >> 3) : ((regPulse2[3] & 0xF8) >> 3);
}

bool APU :: getLengthCounterHalt(int pulse_index){
    return (!pulse_index) ? (regPulse1[0] & 0x20) != 0 : (regPulse2[0] & 0x20) != 0;
}

bool APU :: getEnvelopeFlag(int pulse_index){
    return (!pulse_index) ? (regPulse1[0] & 0x10) != 0 : (regPulse2[0] & 0x10) != 0;
}

double APU :: getVolume (int pulse_index){
    
    unsigned char volume_raw;
    double volume;

    volume_raw = (!pulse_index) ? (regPulse1[0] & 0x0F) : (regPulse2[0] & 0x0F);
    volume = (double) volume_raw / 0x0F;

    return volume;

}

unsigned short APU :: getPulseTimer (int pulse_index){

    unsigned short timer;

    timer = (!pulse_index) ? regPulse1[2] | ((regPulse1[3] & 0x07) << 8) : regPulse2[2] | ((regPulse2[3] & 0x07) << 8);

    return timer;

}

bool APU :: getSweepEnableFlag(int pulse_index){
    return (!pulse_index) ? (regPulse1[1] & 0x80) != 0 : (regPulse2[1] & 0x80) != 0;
}

unsigned char APU :: getSweepDividersPeriod(int pulse_index){
    return (!pulse_index) ? (regPulse1[1] & 0x70) >> 4 : (regPulse2[1] & 0x70) >> 4;
}

bool APU :: getSweepNegativeFlag(int pulse_index){
    return (!pulse_index) ? (regPulse1[1] & 0x04) != 0 : (regPulse2[1] & 0x04) != 0;
}

unsigned char APU::getSweepShiftCount(int pulse_index) {
    if (pulse_index == 0) {
        return (regPulse1[1] >> 0) & 0x07;
    } else {
        return (regPulse2[1] >> 0) & 0x07;
    }
}

float APU::generatePulseSample(int pulse_index) {

    // Get current duty cycle pattern
    unsigned char duty_cycle_raw = (pulse_index == 0) ? (regPulse1[0] & 0xC0) >> 6 : (regPulse2[0] & 0xC0) >> 6;
    const unsigned char* current_duty_sequence = DUTY_CYCLE_SEQUENCES[duty_cycle_raw];

    // Get timer period
    unsigned short timer_period = getPulseTimer(pulse_index);

    // Update timer counter
    if (pulse_timer_counter[pulse_index] == 0) {
        pulse_timer_counter[pulse_index] = timer_period;
        pulse_current_sequence_step[pulse_index] = (pulse_current_sequence_step[pulse_index] + 1) % 8;
    } else {
        pulse_timer_counter[pulse_index]--;

        if (pulse_timer_counter[pulse_index] < 8){
            pulse_output[pulse_index] = 0.0f;
            return pulse_output[pulse_index];
        }
            
    }

    // Get volume
    double volume = getVolume(pulse_index); // This already returns a normalized value (0.0 to 1.0)

    // Determine output based on duty cycle and volume
    if (current_duty_sequence[pulse_current_sequence_step[pulse_index]] == 1) {
        pulse_output[pulse_index] = volume;
    } else {
        //pulse_output[pulse_index] = -volume; // Output -volume when low
        pulse_output[pulse_index] = 0;
    }

    return pulse_output[pulse_index];
}

//Noise wave
bool APU::getNoiseEnvelopeLoop() {
    return (regNoise[0] >> 5) & 0x01;
}

bool APU::getNoiseConstantVolume() {
    return (regNoise[0] >> 4) & 0x01;
}

unsigned char APU::getNoiseVolumeEnvelope() {
    return regNoise[0] & 0x0F;
}

bool APU::getNoiseMode() {
    return (regNoise[2] >> 7) & 0x01;
}

unsigned char APU::getNoisePeriod() {
    return regNoise[2] & 0x0F;
}

unsigned char APU::getNoiseLengthCounterLoad() {
    return (regNoise[3] >> 3) & 0x1F;
}

//Triangular wave
bool APU::getTriangleLengthCounterHalt() {
    return (regTriangle[0] >> 7) & 0x01;
}

unsigned char APU::getTriangleLinearCounterLoad() {
    return regTriangle[0] & 0x7F;
}

unsigned short APU::getTriangleTimer() {
    return (unsigned short)regTriangle[2] | (((unsigned short)regTriangle[3] & 0x07) << 8);
}

unsigned char APU::getTriangleLengthCounterLoad() {
    return (regTriangle[3] >> 3) & 0x1F;
}

float APU::generateTriangleSample() {
    // Get timer period
    unsigned short timer_period = getTriangleTimer();

    // Update timer counter
    if (triangle_timer_counter == 0) {
        triangle_timer_counter = timer_period;
        triangle_current_sequence_step = (triangle_current_sequence_step + 1) % 32;
    } else {
        triangle_timer_counter--;
    }

    // Get output from sequence, normalized to -1 to 1
    // The sequence values are 0-15. To normalize to -1 to 1, we can map 0 to -1, 15 to 1.
    // (value / 7.5) - 1.0
    triangle_output = ((float)TRIANGLE_SEQUENCE[triangle_current_sequence_step] / 7.5f) - 1.0f;

    return triangle_output;
}

float APU::generateNoiseSample() {
    // Get timer period
    unsigned char period_index = getNoisePeriod();
    unsigned short timer_period = NOISE_PERIODS[period_index];

    // Update timer counter
    if (noise_timer_counter == 0) {
        noise_timer_counter = timer_period;

        // Shift LFSR
        unsigned short feedback_bit;
        if (getNoiseMode()) { // Short mode (mode bit is 1)
            feedback_bit = (noise_shift_register & 0x0001) ^ ((noise_shift_register & 0x0040) >> 6); // Bit 0 XOR Bit 6
        } else { // Long mode (mode bit is 0)
            feedback_bit = (noise_shift_register & 0x0001) ^ ((noise_shift_register & 0x0002) >> 1); // Bit 0 XOR Bit 1
        }
        noise_shift_register >>= 1;
        noise_shift_register |= (feedback_bit << 14); // Set bit 14

    } else {
        noise_timer_counter--;
    }

    // Get volume
    unsigned char volume_raw = getNoiseVolumeEnvelope();
    float volume = (float)volume_raw / 15.0f; // Normalize 0-15 to 0-1

    // Determine output based on LFSR's first bit and volume
    if (!(noise_shift_register & 0x0001)) { // If bit 0 is 0
        noise_output = volume;
    } else {
        noise_output = -volume;
    }

    return noise_output;
}

void APU :: start(){
    if (player && !is_playing){
        player->Start();
        player->SetHasData(true);
        is_playing = true;
    }
}

void APU :: stop(){
    if (player && is_playing){
        player->Stop();
        player->SetHasData(false);
        is_playing = false;
    }
}

void APU::AudioCallback(void* cookie, void* buffer, size_t size, const media_raw_audio_format& format) {
    APU* apu = static_cast<APU*>(cookie);
    float* float_buffer = static_cast<float*>(buffer);
    size_t num_samples = size / sizeof(float);

    for (size_t i = 0; i < num_samples; ++i) {
        /*float pulse1_sample = apu->generatePulseSample(0); // Pulse 1
        float pulse2_sample = apu->generatePulseSample(1); // Pulse 2
        float triangle_sample = apu->generateTriangleSample(); // Triangle
        float noise_sample = apu->generateNoiseSample(); // Noise

        // Mix the four channels (simple sum for now)
        // The output should be between -1 and 1.
        // For now, let's just sum and then divide by 4 to keep it within -1 to 1 range.
        float mixed_sample = (pulse1_sample + pulse2_sample + triangle_sample + noise_sample) / 4.0f;*/

        float mixed_sample = (apu->generatePulseSample(0) + apu->generatePulseSample(1)) / 2.0f;

        // Apply overall amplitude
        float_buffer[i] = mixed_sample; // * AMPLITUDE;
    }
}
