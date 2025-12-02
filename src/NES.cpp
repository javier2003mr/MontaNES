#include "NES.h"

NES :: NES (void){
    for (int i = 0; i < CPU_RAM_SIZE; ++i)
        cpu_memory[i] = 0;
}