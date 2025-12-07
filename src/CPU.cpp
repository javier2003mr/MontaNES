#include <stdio.h>
#include "include/CPU.h"

CPU :: CPU (void){ 

    PC = 0xFFFC;

    A = 0;
    X = 0;
    Y = 0;

    SP = 0xFD;

    // Registro de estado
    P = 0x24;

    initializeOpcodeTable();
}

void CPU :: ADC (unsigned char value){
    
    unsigned short sum = A + value + ((getFlag(CARRY)) ? 1 : 0);
    A = static_cast<unsigned char>(sum);
    
    bool overflow = (~(A ^ value) & (A ^ sum)) & 0x80;

    setFlag(CARRY, sum > 0xFF);
    setFlag(ZERO, A == 0);
    setFlag(OVERFLOW, overflow);
    setFlag(NEGATIVE, A & 0x80);
}

void CPU :: AND (unsigned char value){

    A &= value;
    
    setFlag(ZERO, A == 0);
    setFlag(NEGATIVE, A & 0x80);
}

void CPU :: ASL (unsigned char * value){
    
    unsigned char result = *value << 1;
    
    setFlag(CARRY, *value & 0x80);
    setFlag(ZERO, result == 0);
    setFlag(NEGATIVE, result & 0x80);

    *value = result;
}

void CPU :: LSR (unsigned char * value){

    unsigned char result = *value >> 1;

    setFlag(CARRY, *value & 0x01);
    setFlag(ZERO, result == 0);
    setFlag(NEGATIVE, 0);

    *value = result;
}

void CPU :: ROL (unsigned char * value){

    unsigned char result = *value << 1;
    result |= 0x01 & getFlag(CARRY);

    setFlag(CARRY, *value & 0x80);
    setFlag(ZERO, result == 0);
    setFlag(NEGATIVE, result & 0x80);

    *value = result;
}

void CPU :: ROR (unsigned char * value){

    unsigned char result = *value >> 1;
    result |= 0x80 & getFlag(CARRY);

    setFlag(CARRY, *value & 0x01);
    setFlag(ZERO, result == 0);
    setFlag(NEGATIVE, result & 0x80);

    *value = result;
}

void CPU :: BXX (bool flag){
    if (flag)
        PC = cpu_memory[PC+1] << 8 + cpu_memory[PC+2];
}

void CPU :: BCC (){
    BXX(!getFlag(CARRY));
}

void CPU :: BCS (){
    BXX(getFlag(CARRY));
}

void CPU :: BNE (){
    BXX(!getFlag(ZERO));
}

void CPU :: BEQ (){
    BXX(getFlag(ZERO));
}

void CPU :: BPL (){
    BXX(!getFlag(NEGATIVE));
}

void CPU :: BMI (){
    BXX(getFlag(NEGATIVE));
}

void CPU :: BVC (){
    BXX(!getFlag(OVERFLOW));
}

void CPU :: BVS (){
    BXX(getFlag(OVERFLOW));
}

/**************************************************************************************/

// Other functions

/**************************************************************************************/

void CPU :: initializeOpcodeTable(){
    
    for (int i = 0; i < OPCODE_TABLE_SIZE; ++i){
        opcodeTable[i] = {NULL, 0, NULL, IMPLICIT};
    }

    opcodeTable[0x69] = {&CPU::ADC, 2, 2, IMMEDIATE};
    opcodeTable[0x65] = {&CPU::ADC, 2, 3, ZEROPAGE};
    opcodeTable[0x75] = {&CPU::ADC, 2, 4, ZEROPAGE_X};
    opcodeTable[0x6D] = {&CPU::ADC, 3, 4, ABSOLUTE};
    opcodeTable[0x7D] = {&CPU::ADC, 3, 4, ABSOLUTE_X};
    opcodeTable[0x79] = {&CPU::ADC, 3, 4, ABSOLUTE_Y};
    opcodeTable[0x61] = {&CPU::ADC, 2, 6, INDIRECT_X};
    opcodeTable[0x71] = {&CPU::ADC, 2, 5, INDIRECT_Y};

}

bool CPU :: getFlag(Flags flag) {
    return (P & flag) != 0;
}

void CPU :: setFlag(Flags flag, bool condition) {
    if (condition)
        P |= flag;
    else
        P &= ~flag;
}

void CPU :: emulationCycle(){

    unsigned char opcode = cpu_memory[PC];
    unsigned char arg;
    unsigned char C, Z, N, V;

    switch (opcode)
    {
    case 0x69:
    case 0x65:
    case 0x75:
    case 0x6D:
    case 0x7D:
    case 0x79:
    case 0x61:
    case 0x71:

        printf ("ADC\n");
        // 69
        arg = cpu_memory[PC+1];
        
        // 65
        arg = cpu_memory[cpu_memory[PC+1] % 256];

        // 75
        arg = cpu_memory[(cpu_memory[PC+1] + X) % 256];

        // 6D
        arg = cpu_memory[cpu_memory[PC+1] << 8 + cpu_memory[PC+2]];

        // 7D
        arg = cpu_memory[cpu_memory[PC+1] << 8 + cpu_memory[PC+2] + X];

        // 79
        arg = cpu_memory[cpu_memory[PC+1] << 8 + cpu_memory[PC+2] + Y];

        // 61	
        arg = cpu_memory[cpu_memory[(cpu_memory[PC+1] + X) % 256] + cpu_memory[(cpu_memory[PC+1] + X + 1) % 256] * 256];

        // 71
        arg = cpu_memory[cpu_memory[cpu_memory[PC+1] % 256] + cpu_memory[(cpu_memory[PC+1] + 1) % 256] * 256 + Y];

        C = P & 0x01;
        A += arg + C;
        
        break;
    
    case 0x29:
    case 0x25:
    case 0x35:
    case 0x2D:
    case 0x3D:
    case 0x39:
    case 0x21:
    case 0x31:

        printf ("AND\n");
        // 29
        arg = cpu_memory[PC+1];
        
        // 25
        arg = cpu_memory[cpu_memory[PC+1] % 256];

        // 35
        arg = cpu_memory[(cpu_memory[PC+1] + X) % 256];

        // 2D
        arg = cpu_memory[cpu_memory[PC+1] << 8 + cpu_memory[PC+2]];

        // 3D
        arg = cpu_memory[cpu_memory[PC+1] << 8 + cpu_memory[PC+2] + X];

        // 39
        arg = cpu_memory[cpu_memory[PC+1] << 8 + cpu_memory[PC+2] + Y];

        // 31
        arg = cpu_memory[cpu_memory[(cpu_memory[PC+1] + X) % 256] + cpu_memory[(cpu_memory[PC+1] + X + 1) % 256] * 256];

        // 71
        arg = cpu_memory[cpu_memory[cpu_memory[PC+1] % 256] + cpu_memory[(cpu_memory[PC+1] + 1) % 256] * 256 + Y];

        C = P & 0x01;
        A &= arg;

        break;

    case 0x0A:
    case 0x06:
    case 0x16:
    case 0x0E:
    case 0x1E:

        printf ("ASL\n");
        // 0A
        A = A << 1;
        
        // 06
        cpu_memory[cpu_memory[PC+1] % 256] = cpu_memory[cpu_memory[PC+1] % 256] << 1;

        // 16
        cpu_memory[(cpu_memory[PC+1] + X) % 256] = cpu_memory[(cpu_memory[PC+1] + X) % 256] << 1;

        // 0E
        cpu_memory[cpu_memory[PC+1] << 8 + cpu_memory[PC+2]] = cpu_memory[cpu_memory[PC+1] << 8 + cpu_memory[PC+2]] << 1;

        // 1E
        cpu_memory[cpu_memory[PC+1] << 8 + cpu_memory[PC+2] + X] = cpu_memory[cpu_memory[PC+1] << 8 + cpu_memory[PC+2] + X] << 1;

        break;

    case 0x90:

        printf ("BCC\n");
        // 90
        C = P & 0x01;
        arg = cpu_memory[PC+1] << 8 + cpu_memory[PC+2];
        
        if (!C)
            PC = arg;

        break;

    case 0xB0:
        printf ("BCS\n");
        // B0
        C = P & 0x01;
        arg = cpu_memory[PC+1] << 8 + cpu_memory[PC+2];
        
        if (C)
            PC = arg;

        break;

    case 0xF0:
        printf ("BEQ\n");
        // B0
        Z = P & 0x02;
        arg = cpu_memory[PC+1] << 8 + cpu_memory[PC+2];
        
        if (Z)
            PC = arg;

        break;

    case 0x30:
        printf ("BMI\n");
        // 30
        N = P & 0x80;
        arg = cpu_memory[PC+1] << 8 + cpu_memory[PC+2];
        
        if (N)
            PC = arg;

        break;
    
    case 0x10:
        printf ("BPL\n");
        // 10
        N = P & 0x80;
        arg = cpu_memory[PC+1] << 8 + cpu_memory[PC+2];
        
        if (!N)
            PC = arg;

        break;

    case 0xD0:
        printf ("BNE\n");
        // D0
        Z = P & 0x02;
        arg = cpu_memory[PC+1] << 8 + cpu_memory[PC+2];
        
        if (!Z)
            PC = arg;
        
        break;
    case 0x50:
        printf ("BVC\n");
        // 50
        V = P & 0x40;
        arg = cpu_memory[PC+1] << 8 + cpu_memory[PC+2];
        
        if (!V)
            PC = arg;

        break;

    case 0x70:
        printf ("BVS\n");
        // 50
        V = P & 0x40;
        arg = cpu_memory[PC+1] << 8 + cpu_memory[PC+2];
        
        if (V)
            PC = arg;

        break;
    default:
        break;
    }

}