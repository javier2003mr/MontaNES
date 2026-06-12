#include <stdio.h>
#include <functional>
#include "../include/CPU.h"
#include "../include/PPU.hpp"

CPU :: CPU (void){ 

    initializeOpcodeTable();
    printf("Opcode Table Initialized\n");
}

void CPU :: reset (void) {
    
    PC = getMemoryValue(0xFFFD) << 8 | getMemoryValue(0xFFFC);
    printf("Start address: %x\n", PC);
    
    A = 0;
    X = 0;
    Y = 0;

    SP = 0xFD;

    // Registro de estado
    P = 0x24;

    modifyPC = true;
}

void CPU :: ADC (unsigned char value){
    
    unsigned short sum = A + value + ((getFlag(C_FLAG)) ? 1 : 0);    
    bool overflow = (~(A ^ value) & (A ^ sum)) & 0x80;

    A = static_cast<unsigned char>(sum);

    setFlag(C_FLAG, sum > 0xFF);
    setFlag(Z_FLAG, A == 0);
    setFlag(V_FLAG, overflow);
    setFlag(N_FLAG, A & 0x80);
}

void CPU :: SBC(unsigned char value) {
    
    unsigned char borrow = 1 - getFlag(C_FLAG);
    
    unsigned char aux = A - value - borrow;
    
    setFlag(Z_FLAG, aux == 0);
    setFlag(N_FLAG, aux & 0x80);
    setFlag(C_FLAG, A >= value + borrow); 
    setFlag(V_FLAG, ((A ^ value) & 0x80) && ((A ^ aux) & 0x80));
    
    A = aux;
}

void CPU :: INC (unsigned short dir){
    
    unsigned char aux = getMemoryValue(dir);
    ++aux;
    setMemoryValue(dir, aux);

    //unsigned char aux = ++cpu_memory[dir];

    setFlag(N_FLAG, aux & 0x80);
    setFlag(Z_FLAG, aux == 0);
}

void CPU :: DEC (unsigned short dir){

    unsigned char aux = getMemoryValue(dir);
    --aux;
    setMemoryValue(dir, aux);

    //unsigned char aux = --cpu_memory[dir];

    setFlag(N_FLAG, aux & 0x80);
    setFlag(Z_FLAG, aux == 0);
}

void CPU :: INX (){
    ++X;
    setFlag(N_FLAG, X & 0x80);
    setFlag(Z_FLAG, X == 0);
}

void CPU :: DEX (){
    --X;
    setFlag(N_FLAG, X & 0x80);
    setFlag(Z_FLAG, X == 0);
}

void CPU :: INY (){
    ++Y;
    setFlag(N_FLAG, Y & 0x80);
    setFlag(Z_FLAG, Y == 0);
}

void CPU :: DEY (){
    --Y;
    setFlag(N_FLAG, Y & 0x80);
    setFlag(Z_FLAG, Y == 0);
}

void CPU :: AND (unsigned char value){

    A &= value;
    
    setFlag(Z_FLAG, A == 0);
    setFlag(N_FLAG, A & 0x80);
}

void CPU :: ORA (unsigned char value){
    A |= value;

    setFlag(Z_FLAG, A == 0);
    setFlag(N_FLAG, A & 0x80);
}

void CPU :: EOR (unsigned char value){

    A ^= value;

    setFlag(Z_FLAG, A == 0);
    setFlag(N_FLAG, A & 0x80);
}

void CPU :: BIT (unsigned char value){

    unsigned char aux = A & value;
    
    setFlag(Z_FLAG, aux == 0);
    setFlag(V_FLAG, value & 0x40);
    setFlag(N_FLAG, value & 0x80);
}

void CPU :: ASL (unsigned char * value){
    
    unsigned char result = *value << 1;
    
    setFlag(C_FLAG, *value & 0x80);
    setFlag(Z_FLAG, result == 0);
    setFlag(N_FLAG, result & 0x80);

    *value = result;
}

void CPU :: LSR (unsigned char * value){

    unsigned char result = *value >> 1;

    setFlag(C_FLAG, *value & 0x01);
    setFlag(Z_FLAG, result == 0);
    setFlag(N_FLAG, 0);

    *value = result;
}

void CPU :: ROL (unsigned char * value){

    unsigned char result = *value << 1;
    result |= 0x01 & getFlag(C_FLAG);

    setFlag(C_FLAG, *value & 0x80);
    setFlag(Z_FLAG, result == 0);
    setFlag(N_FLAG, result & 0x80);

    *value = result;
}

void CPU :: ROR (unsigned char * value){

    unsigned char result = *value >> 1;
    result = (getFlag(C_FLAG)) ? 0x80 | result : 0x7F & result;

    setFlag(C_FLAG, *value & 0x01);
    setFlag(Z_FLAG, result == 0);
    setFlag(N_FLAG, result & 0x80);

    *value = result;
}

void CPU :: CMP_GEN (unsigned char value, unsigned char reg){
    setFlag(C_FLAG, reg >= value);
    setFlag(Z_FLAG, reg == value);
    setFlag(N_FLAG, (reg - value) & 0x80);
}

void CPU :: CMP (unsigned char value){

    CMP_GEN(value, A);
}

void CPU :: CPX (unsigned char value){

    CMP_GEN(value, X);
}

void CPU :: CPY (unsigned char value){

    CMP_GEN(value, Y);
}

void CPU :: BXX (bool flag){

    if (flag){
        unsigned short current_pc;
        current_pc = PC + 2;

        PC += 2 + static_cast<char>(getMemoryValue(PC+1));

        modifyPC = false;

        // takes the branch so cycles + 1
        ++instruction_cycles;

        // crosses the branch so cycles + 2
        if ((current_pc & 0xFF00) != (PC & 0xFF00))
            ++instruction_cycles;
    }
}

void CPU :: BCC (){
    BXX(!getFlag(C_FLAG));
}

void CPU :: BCS (){
    BXX(getFlag(C_FLAG));
}

void CPU :: BNE (){
    BXX(!getFlag(Z_FLAG));
}

void CPU :: BEQ (){
    BXX(getFlag(Z_FLAG));
}

void CPU :: BPL (){
    BXX(!getFlag(N_FLAG));
}

void CPU :: BMI (){
    BXX(getFlag(N_FLAG));
}

void CPU :: BVC (){
    BXX(!getFlag(V_FLAG));
}

void CPU :: BVS (){
    BXX(getFlag(V_FLAG));
}

void CPU :: JMP (unsigned short dir){
    PC = dir;
    modifyPC = false;
}

void CPU :: JSR (unsigned short dir){
    
    stack_push(((PC+2) & 0xFF00) >> 8);
    stack_push((PC+2) & 0x00FF);
    PC = dir;
    modifyPC = false;
}

void CPU :: RTS (){

    unsigned char low_byte = stack_pop();
    unsigned char high_byte = stack_pop();
    PC = (high_byte << 8) | low_byte;
    ++PC;
    modifyPC = false;
}

void CPU :: RTI (){
    unsigned char aux = stack_pop();
    P = (aux & 0xCF) | (P & 0x30);
    RTS();
    --PC;
}

void CPU :: BRK (){
    stack_push(((PC+2) & 0xFF00) >> 8);
    stack_push((PC+2) & 0x00FF);
    stack_push(P | 0x30);
    PC = getMemoryValue(0xFFFF) << 8 | getMemoryValue(0xFFFE);
    modifyPC = false;
    P |= 0x04;
}

void CPU :: TAX (){
    
    X = A;
    setFlag(Z_FLAG, A == 0);
    setFlag(N_FLAG, A & 0x80);
}

void CPU :: TAY (){

    Y = A;
    setFlag(Z_FLAG, Y == 0);
    setFlag(N_FLAG, Y & 0x80);
}

void CPU :: TXA (){
    
    A = X;
    setFlag(Z_FLAG, X == 0);
    setFlag(N_FLAG, X & 0x80);
}

void CPU :: TYA (){
    
    A = Y;
    setFlag(Z_FLAG, A == 0);
    setFlag(N_FLAG, A & 0x80);
}

void CPU :: TSX (){
    
    X = SP;
    
    setFlag(N_FLAG, X & 0x80);
    setFlag(Z_FLAG, X == 0);
}

void CPU :: TXS (){
    SP = X;
}

void CPU :: PHA (){
    stack_push(A);
}

void CPU :: PLA (){
    A = stack_pop();

    setFlag(N_FLAG, A & 0x80);
    setFlag(Z_FLAG, A == 0);
}

void CPU :: PHP (){
    stack_push(P | 0x30);
}

void CPU :: PLP (){
    unsigned char aux = stack_pop();
    P = (aux & 0xCF) | (P & 0x30);
}

void CPU :: LDA (unsigned short dir){
    A = getMemoryValue(dir);
    setFlag(Z_FLAG, A == 0);
    setFlag(N_FLAG, A & 0x80);
}

void CPU :: STA (unsigned short dir){

    setMemoryValue(dir, A);
    //cpu_memory[dir] = A;
}

void CPU :: LDX (unsigned short dir){
    X = getMemoryValue(dir);
    setFlag(Z_FLAG, X == 0);
    setFlag(N_FLAG, X & 0x80);
}

void CPU :: STX (unsigned short dir){

    setMemoryValue(dir, X);
    //cpu_memory[dir] = X;
}

void CPU :: LDY (unsigned short dir){
    Y = getMemoryValue(dir);
    setFlag(Z_FLAG, Y == 0);
    setFlag(N_FLAG, Y & 0x80);
}

void CPU :: STY (unsigned short dir){
    
    setMemoryValue(dir, Y);
    //cpu_memory[dir] = Y;
}

void CPU :: CLC (){
    setFlag(C_FLAG, 0);
}

void CPU :: SEC (){
    setFlag(C_FLAG, 1);
}

void CPU :: CLI (){
    setFlag(I_FLAG, 0);
}

void CPU :: SEI (){
    setFlag(I_FLAG, 1);
}

void CPU :: CLD (){
    setFlag(D_FLAG, 0);
}

void CPU :: SED (){
    setFlag(D_FLAG, 1);
}

void CPU :: CLV (){
    setFlag(V_FLAG, 0);
}

void CPU :: NOP () {}
void CPU :: STP () {modifyPC = false;}

void CPU :: SLO (unsigned char * value) {
    ASL(value);
    ORA(*value);
}

void CPU :: ANC (unsigned char value) {
    AND(value);
    setFlag(C_FLAG, getFlag(N_FLAG));
}

void CPU :: RLA (unsigned char * value) {
    ROL(value);
    AND(*value);
}

void CPU :: SRE (unsigned char * value) {
    LSR(value);
    EOR(*value);
}

void CPU :: ASR (unsigned char * value){
    AND(*value);
    LSR(&A);
}

void CPU :: RRA (unsigned char * value){
    ROR(value);
    ADC(*value);
}

void CPU :: ARR (unsigned char * value){
    AND(*value);
    ROR(&A);
    setFlag(V_FLAG, ((A & 0x20) >> 5) != ((A & 0x40) >> 6));
    setFlag(C_FLAG, A & 0x40);
}

void CPU :: SAX (unsigned char * value){
    *value = A & X;
}

void CPU :: XAA (unsigned char value){
    A = (0xEE | A) & X & value;

    setFlag(Z_FLAG, A == 0);
    setFlag(N_FLAG, A & 0x80);
}

void CPU :: SHX (unsigned short dir){
    
    setMemoryValue(dir, X & (getMemoryValue(PC+2) + 1));
    //cpu_memory[dir] = X & (cpu_memory[PC+2] + 1);
}

void CPU :: SHY (unsigned short dir){

    setMemoryValue(dir, Y & (getMemoryValue(PC+2) + 1));
    //cpu_memory[dir] = Y & (cpu_memory[PC+2] + 1);
}

void CPU :: LAX (unsigned short dir){
    LDA(dir);
    LDX(dir);
}

void CPU :: LAS (unsigned char value){
    
    unsigned char aux = value & SP;

    A = aux;
    X = aux;
    SP = aux;

    setFlag(N_FLAG, A & 0x80);
    setFlag(Z_FLAG, A == 0);
}

void CPU :: DCP (unsigned char * value){
    
    --(*value);

    unsigned char aux = A - (*value);

    setFlag(Z_FLAG, aux == 0);
    setFlag(N_FLAG, aux & 0x80);
    setFlag(C_FLAG, (*value) <= A);
}

void CPU :: SBX (unsigned char value) {
    
    unsigned char aux = (A & X);

    CMP_GEN (value, aux);
    
    X = (A & X) - value;
}

void CPU :: ISC(unsigned char *value) {
    
    ++(*value);
    
    unsigned char operand = *value;
    unsigned char borrow = getFlag(C_FLAG) ? 0 : 1;
    
    unsigned int temp = A - operand - borrow;
    
    setFlag(C_FLAG, A >= operand + borrow);
    setFlag(Z_FLAG, (temp & 0xFF) == 0);
    setFlag(N_FLAG, temp & 0x80);
    
    unsigned char overflow = ((A ^ operand) & 0x80) && ((A ^ temp) & 0x80);
    setFlag(V_FLAG, overflow);
    
    A = temp & 0xFF;
}

void CPU :: SHA_absoluteY(unsigned short dir) {
    
    unsigned char high_byte = (dir >> 8) + 1;
    unsigned char value = A & X & high_byte;
    
    setMemoryValue(dir, value);
    //cpu_memory[dir] = value;
}

void CPU :: SHA_indirectY(unsigned char zp_addr) {

    unsigned short base_addr = getMemoryValue(zp_addr) | (getMemoryValue((zp_addr + 1) & 0xFF) << 8);
    
    unsigned short effective_addr = base_addr + Y;
    unsigned char high_byte = (base_addr >> 8) + 1;
    unsigned char value = A & X & high_byte;
    
    unsigned short store_addr = ((high_byte & A) << 8) | (effective_addr & 0xFF);
    
    setMemoryValue(store_addr, value);
    //cpu_memory[store_addr] = value;
}

void CPU :: SHA (unsigned short dir) {
    
    unsigned char high_byte = (dir >> 8) & 0xFF;
    unsigned char result = A & X & (high_byte + 1);

    setMemoryValue(dir, result);
    //cpu_memory[dir] = result;
}

void CPU :: nmi() {

    stack_push((PC >> 8) & 0xFF);
    stack_push(PC & 0xFF);

    setFlag(B_FLAG, false);
    stack_push(P);
    setFlag(I_FLAG, true);

    PC = getMemoryValue(0xFFFA) | (getMemoryValue(0xFFFB) << 8);
}

void CPU :: irq(){
    BRK();
    modifyPC = true;
}

void CPU :: connectPPU(PPU* ppu_ptr) {
    this->ppu = ppu_ptr;
}

void CPU :: connectJoypad (Joypad * joypad_ptr){
    this->joypad = joypad_ptr;
}

void CPU :: connectCartridge (Cartridge * c){
    this->cartridge = c;
}

void CPU :: connectAPU (APU * a){
    this->apu = a;
}

/**************************************************************************************/

// Other functions

/**************************************************************************************/

void CPU :: stack_push (unsigned char data){
    
    setMemoryValue(0x0100 | SP, data);
    --SP;
}

unsigned char CPU :: stack_pop (){

    ++SP;
    return getMemoryValue(0x0100 | SP);
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

void CPU :: setPC(unsigned short value){ PC = value; }

void CPU :: setA (unsigned char value){ A = value; }

void CPU :: setP (unsigned char value){ P = value; }

void CPU :: setSP(unsigned short value){ SP = value; }

void CPU :: setX (unsigned char value){ X = value; }

void CPU :: setY (unsigned char value){ Y = value; }

void CPU :: setMemoryValue (unsigned short dir, unsigned char value){ 
    
    if (dir < CPU_RAM_SIZE){ 

        if (dir < 0x2000){
            
            dir = dir % 0x800;
            cpu_memory[dir] = value; 

        } else if (dir >= 0x2000 && dir < 0x4000) {
            if (this->ppu == nullptr) {
                printf("CPU::setMemoryValue - ERROR: PPU pointer is nullptr when writing to PPU register 0x%04X\n", dir);
                return;
            }
            
            switch (0x2000 + (dir % 8)) {
                case 0x2000:
                    this->ppu->setPPUCTRL(value);
                    break;
                case 0x2001:
                    this->ppu->setPPUMASK(value);
                    break;
                case 0x2003:
                    this->ppu->setOAMADDR(value);
                    break;
                case 0x2004:
                    this->ppu->setOAMDATA(value);
                    break;
                case 0x2005:
                    this->ppu->setPPUSCROLL(value);
                    break;
                case 0x2006:
                    this->ppu->setPPUADDR(value);
                    break;
                case 0x2007:
                    this->ppu->setPPUDATA(value);
                    break;
            }
        } else if (dir == 0x4014) {
            if (this->ppu == nullptr) {
                printf("CPU::setMemoryValue - ERROR: PPU pointer is nullptr when writing to OAMDMA 0x%04X\n", dir);
                return;
            }
            this->ppu->setOAMDMA(value);
        } else if (dir == 0x4016) {
            joypad->write4016(value);
            cpu_memory[dir] = value;
        } else if (dir >= 0x4000 && dir <= 0x4017){
            apu->setAudioValue(dir, value);
        }else if (dir >= 0x8000){
            cartridge->catchWriteInRAM(dir, value);
        } else {
            cpu_memory[dir] = value; 
        }
    } 
}

unsigned short CPU :: getPC(){ return PC; }

unsigned char CPU :: getA (){ return A; }

unsigned char CPU :: getP (){ return P; }

unsigned short CPU :: getSP(){ return SP; }

unsigned char CPU :: getX (){ return X; }

unsigned char CPU :: getY (){ return Y; }

unsigned char CPU :: getMemoryValue (unsigned short dir){ 

    if (dir < CPU_RAM_SIZE) {

        if (dir < 0x2000){
            
            dir = dir % 0x800;

        }else if (dir < 0x4000) {
            
            switch (0x2000 + (dir % 8)) {
                case 0x2002:
                    return this->ppu->getPPUSTATUS();
                case 0x2004:
					return this->ppu->getOAMDATA();
                case 0x2007:
                    return this->ppu->getPPUDATA();
            }
        }else if (dir == 0x4016){
            return joypad->read4016();
        }else if (dir >= 0x4000 && dir <= 0x4017 && dir != 0x4014){
            return apu->getAudioValue(dir);
        }else if (dir >= 0x8000){
            return cartridge->getPRGValue(dir);
        }

        return cpu_memory[dir];

    }
}

unsigned char * CPU :: getMemoryDir (unsigned short dir){
    
    if (dir < CPU_RAM_SIZE) {

        if (dir < 0x2000){
            
            dir = dir % 0x800;

        }else if (dir < 0x4000) {
            
            dir = 0x2000 | (dir % 0x8);

        }else if (dir >= 0x8000) {
            
            return cartridge->getPRGDir(dir);

        }

        return &cpu_memory[dir];

    }
}

void CPU :: executeOpcode(OpcodeInfo & info, void* param = nullptr) {

    if (!info.handler.func.void_func) {
        // Operacion ilegal o no definida
        return;
    }
    
    switch (info.handler.type) {
        case InstructionHandler::TYPE_VOID:
            (this->*(info.handler.func.void_func))();
            break;
            
        case InstructionHandler::TYPE_UCHAR:
            (this->*(info.handler.func.uchar_func))(*static_cast<unsigned char*>(param));
            break;
            
        case InstructionHandler::TYPE_USHORT:
            (this->*(info.handler.func.ushort_func))(*static_cast<unsigned short*>(param));
            break;
            
        case InstructionHandler::TYPE_UCHAR_PTR:
            (this->*(info.handler.func.uchar_ptr_func))(static_cast<unsigned char*>(param));
            break;
            
        case InstructionHandler::TYPE_BOOL:
            (this->*(info.handler.func.bool_func))(*static_cast<bool*>(param));
            break;
    }
}

void CPU :: readArgs(OpcodeInfo & info, unsigned char * & arg, unsigned short & aux){

    int pc_plus_1 = (PC + 1) % CPU_RAM_SIZE;
    int pc_plus_2 = (PC + 2) % CPU_RAM_SIZE;

    switch (info.mode)
    {
    case IMPLIED:
        arg = nullptr;
        break;
    
    case ACCUMULATOR:
        arg = &A;
        break;

    case IMMEDIATE:
        aux = PC + 1;
        arg = getMemoryDir(aux);
        break;
    
    case ZEROPAGE:
        aux = getMemoryValue(pc_plus_1) % 256;
        arg = getMemoryDir(aux);
        break;

    case ZEROPAGE_X:
        aux = (getMemoryValue(pc_plus_1) + X) % 256;
        arg = getMemoryDir(aux);
        break;

    case ZEROPAGE_Y:
        aux = (getMemoryValue(pc_plus_1) + Y) % 256;
        arg = getMemoryDir(aux);
        break;
    
    case ABSOLUTE:
        aux = (getMemoryValue(pc_plus_2) << 8) + getMemoryValue(pc_plus_1);
        arg = getMemoryDir(aux);
        break;
    
    case ABSOLUTE_X:
        aux = (getMemoryValue(pc_plus_2) << 8) + getMemoryValue(pc_plus_1);
        
        if ((aux & 0xFF00) != ((aux+X) & 0xFF00) && info.handler.func.uchar_ptr_func != &CPU::ASL && info.handler.func.ushort_func != &CPU::DEC &&
            info.handler.func.ushort_func != &CPU::INC && info.handler.func.uchar_ptr_func != &CPU::LSR && info.handler.func.uchar_ptr_func != &CPU::ROL &&
            info.handler.func.uchar_ptr_func != &CPU::ROR && info.handler.func.ushort_func != &CPU::STA)
            ++instruction_cycles;

        aux = (aux+X)%CPU_RAM_SIZE;
        arg = getMemoryDir(aux);
        break;
    
    case ABSOLUTE_Y:
        aux = (getMemoryValue(pc_plus_2) << 8) + getMemoryValue(pc_plus_1);
        
        if ((aux & 0xFF00) != ((aux+Y) & 0xFF00) && info.handler.func.ushort_func != &CPU::STA)
            ++instruction_cycles;
        
        aux = (aux+Y)%CPU_RAM_SIZE;
        arg = getMemoryDir(aux);
        break;
    case INDIRECT:
        aux = (getMemoryValue(pc_plus_2) << 8) + getMemoryValue(pc_plus_1);
        aux = (getMemoryValue(((aux & 0x00FF) < 0xFF) ? aux+1 : aux & 0xFF00) << 8) + getMemoryValue(aux);
        arg = getMemoryDir(aux);
        break;
    case INDIRECT_X:
        aux = (getMemoryValue(pc_plus_1) + X) % 256;
        aux = (getMemoryValue((aux+1) % 256) << 8) + getMemoryValue(aux);
        arg = getMemoryDir(getMemoryValue((getMemoryValue(pc_plus_1) + X) % 256) + getMemoryValue((getMemoryValue(pc_plus_1) + X + 1) % 256) * 256);
        break;
    case INDIRECT_Y:
        aux = getMemoryValue(pc_plus_1);
        aux = (getMemoryValue((aux+1) % 256) << 8) + getMemoryValue(aux % 256);
        
        if ((aux & 0xFF00) != ((aux+Y) & 0xFF00) && info.handler.func.ushort_func != &CPU::STA)
            ++instruction_cycles;
        
        aux = (aux+Y)%CPU_RAM_SIZE;
        arg = getMemoryDir(aux);
        break;

    case RELATIVE:
        arg = nullptr;
        break;
    
    default:
        arg = nullptr;
        break;
    }

}

int CPU :: emulationCycle(){
    
    instruction_cycles = 0;

    unsigned char opcode = getMemoryValue(PC);

    OpcodeInfo info = opcodeTable[opcode];
    
    unsigned char * arg;
    unsigned short aux;

    readArgs(info, arg, aux);
    
    if (info.handler.type == InstructionHandler::TYPE_USHORT)
        executeOpcode(info, &aux);
    else
        executeOpcode(info, arg);


    if (modifyPC)
        PC += info.length;

    modifyPC = true;

    instruction_cycles += info.cycles;
    return instruction_cycles;
}