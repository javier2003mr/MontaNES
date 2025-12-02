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
}

void CPU :: emulationCycle(){

    unsigned char opcode = cpu_memory[PC];
    unsigned char arg;
    unsigned char C;

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

        //61
        //val = PEEK(PEEK((arg + X) % 256) + PEEK((arg + X + 1) % 256) * 256) 	
        arg = cpu_memory[cpu_memory[(cpu_memory[PC+1] + X) % 256] + cpu_memory[(cpu_memory[PC+1] + X + 1) % 256] * 256];
        C = P & 0x01;
        A += arg + C;
        

        /* code */
        break;
    
    default:
        break;
    }

}