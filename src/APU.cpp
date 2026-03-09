#include "APU.hpp"

void APU :: reset(){
    
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