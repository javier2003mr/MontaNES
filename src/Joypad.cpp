#include "Joypad.hpp"

Joypad :: Joypad (){
    strove_active = 0;
    buttons = 0xFF;
    shift_register = 0xFF;
}

void Joypad :: loadButtons (bool a, bool b, bool select, bool start, bool up, bool down, bool left, bool right){
    
    buttons = 0;

    buttons |= (a) ? 0x01 : 0;
    buttons |= (b) ? 0x02 : 0;
    buttons |= (select) ? 0x04 : 0;
    buttons |= (start) ? 0x08 : 0;
    buttons |= (up) ? 0x10 : 0;
    buttons |= (down) ? 0x20 : 0;
    buttons |= (left) ? 0x40 : 0;
    buttons |= (right) ? 0x80 : 0;

}

void Joypad :: write4016 (unsigned char value){
    
    bool new_strobe = (value & 0x01 != 0);

    // Flanco de bajada del strobe (carga datos en el shift register)
    if (strove_active &&  !new_strobe){
        shift_register = buttons;
    }

    strove_active = new_strobe;
}

unsigned char Joypad :: read4016 (){

    unsigned char ret = shift_register & 0x01;

    // En caso de ser 1 este carga siempre el mismo valor
    // Si es 0, desplazará el registro
    if (!strove_active){   
        shift_register >>= 1;
    }

    return ret;
}