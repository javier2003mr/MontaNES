#include "APU.hpp"
#include <stdio.h>

const int32 SAMPLE_RATE = 44100;
const size_t BUFFER_SIZE = 1250 * sizeof(float);
const float AMPLITUDE = 0.1f;
const float FREQUENCY = 440.0f; // A4 note

APU :: APU(){
    player = nullptr;
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


void APU :: start(){
    if (player && !is_playing){
        player->Start();
        is_playing = true;
    }
}

void APU :: stop(){
    if (player && is_playing){
        player->Stop();
        is_playing = false;
    }
}

void APU::AudioCallback(void* cookie, void* buffer, size_t size, const media_raw_audio_format& format) {

    //Pulse audio
    APU * apu = static_cast<APU*>(cookie);

    /*Duty cycle: 
    00: 12'5% -------> 0 1 0 0 0 0 0 0
    01: 25%, --------> 0 1 1 0 0 0 0 0 
    10: 50%, --------> 0 1 1 1 1 0 0 0
    11: %75 ---------> 1 0 0 1 1 1 1 1
    */

    apu->getDutyCycle(0);
    apu->getDutyCycle(1);
}
