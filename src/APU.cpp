#include "APU.hpp"

const int32 SAMPLE_RATE = 44100;
const size_t BUFFER_SIZE = 1250 * sizeof(float);
const float AMPLITUDE = 0.1f;
const float FREQUENCY = 440.0f; // A4 note

void APU :: reset(){
    
    // Inicialización de los registros
    for (int i = 0; i < 4; ++i){
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

    //Parte del codigo de BSoundPlayer
    is_playing = false;
    
    media_raw_audio_format format;

    format.frame_rate = SAMPLE_RATE;
    format.channel_count = 1; // Mono
    format.format = media_raw_audio_format::B_AUDIO_FLOAT;
    format.buffer_size = BUFFER_SIZE; // 10ms buffer
    format.byte_order = B_MEDIA_LITTLE_ENDIAN;

    player = new BSoundPlayer (&format, "MontaNES Audio", &APU::AudioCallback, NULL, this);

}

unsigned char APU :: getAudioValue(unsigned short dir){

    dir = dir - 0x4000;

    if (dir < 0x04){
        return regPulse1[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x08){
        return regPulse2[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x0C){
        return regTriangle[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x10){
        return regNoise[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x15){
        return regDMC[dir % TAM_APU_REGISTERS];
    }else if (dir == 0x15){
        return reg4015;
    }else if (dir == 0x17){
        return reg4017;
    }

}

unsigned char * APU :: getAudioDir(unsigned short dir){

    dir = dir - 0x4000;

    if (dir < 0x04){
        return &regPulse1[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x08){
        return &regPulse2[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x0C){
        return &regTriangle[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x10){
        return &regNoise[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x15){
        return &regDMC[dir % TAM_APU_REGISTERS];
    }else if (dir == 0x15){
        return &reg4015;
    }else if (dir == 0x17){
        return &reg4017;
    }

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

double APU :: getVolume (int pulse_index){
    
    unsigned char volume_raw;
    double volume;

    volume_raw = (!pulse_index) ? (regPulse1[0] & 0x0F) : (regPulse2[0] & 0x0F);
    volume = (double) volume_raw / 0x0F;

    return volume;

}

double APU :: getPulseTimer (int pulse_index){

    unsigned short timer;

    timer = (!pulse_index) ? regPulse1[2] & ((regPulse1[3] & 0x07) << 8) : regPulse2[2] & ((regPulse2[3] & 0x07) << 8);

    return timer;

}

void APU::AudioCallback(void* cookie, void* buffer, size_t size, const media_raw_audio_format& format) {

    //Pulse audio

    //Duty cycle ----> 00: 12'5%, 01: 25%, 10: 50%, 11: %75
    getDutyCycle(0);
    getDutyCycle(1);
}
