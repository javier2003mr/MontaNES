#include <stdio.h>
#include <functional>
#include "../include/CPU.h"

CPU :: CPU (void){ 

    PC = 0xFFFC;

    A = 0;
    X = 0;
    Y = 0;

    SP = 0xFD;

    // Registro de estado
    P = 0x24;

    modifyPC = true;

    initializeOpcodeTable();
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

void CPU :: SBC (unsigned char value){
    
    unsigned short result = A + (~value) + ((getFlag(C_FLAG)) ? 1 : 0);
    bool overflow = ((A ^ result) & (A ^ (~value)) & 0x80) != 0;
    
    A = static_cast<unsigned char>(result);

    setFlag(C_FLAG, result <= 0xFF);
    setFlag(Z_FLAG, A == 0);
    setFlag(V_FLAG, overflow);
    setFlag(N_FLAG, A & 0x80);
}

void CPU :: INC (unsigned short dir){
    ++cpu_memory[dir];
}

void CPU :: DEC (unsigned short dir){
    --cpu_memory[dir];
}

void CPU :: INX (){
    ++X;
}

void CPU :: DEX (){
    --X;
}

void CPU :: INY (){
    ++Y;
}

void CPU :: DEY (){
    --Y;
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
    result |= 0x80 & getFlag(C_FLAG);

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

        PC += 2 + static_cast<char>(cpu_memory[PC+1]);

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
    PC = high_byte << 8 | low_byte;
    modifyPC = false;
}

void CPU :: RTI (){
    unsigned char aux = stack_pop();
    P = (aux & 0xCF) | (P & 0x30);
    RTS();
}

void CPU :: BRK (){
    stack_push(((PC+2) & 0xFF00) >> 8);
    stack_push((PC+2) & 0x00FF);
    stack_push(P | 0x30);
    PC = cpu_memory[0xFFFF] << 8 | cpu_memory[0xFFFE];
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
}

void CPU :: TXS (){
    SP = X;
}

void CPU :: PHA (){
    stack_push(A);
}

void CPU :: PLA (){
    A = stack_pop();
}

void CPU :: PHP (){
    stack_push(P | 0x30);
}

void CPU :: PLP (){
    unsigned char aux = stack_pop();
    P = (aux & 0xCF) | (P & 0x30);
}

void CPU :: LDA (unsigned short dir){
    A = cpu_memory[dir];
    setFlag(Z_FLAG, A == 0);
    setFlag(N_FLAG, A & 0x80);
}

void CPU :: STA (unsigned short dir){
    cpu_memory[dir] = A;
}

void CPU :: LDX (unsigned short dir){
    X = cpu_memory[dir];
    setFlag(Z_FLAG, X == 0);
    setFlag(N_FLAG, X & 0x80);
}

void CPU :: STX (unsigned short dir){
    cpu_memory[dir] = X;
}

void CPU :: LDY (unsigned short dir){
    Y = cpu_memory[dir];
    setFlag(Z_FLAG, Y == 0);
    setFlag(N_FLAG, Y & 0x80);
}

void CPU :: STY (unsigned short dir){
    cpu_memory[dir] = Y;
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

/**************************************************************************************/

// Other functions

/**************************************************************************************/

void CPU :: stack_push (unsigned char data){
    
    cpu_memory[0x0100 | SP] = data;
    --SP;
}

unsigned char CPU :: stack_pop (){

    ++SP;
    return cpu_memory[0x0100 | SP];
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

void CPU :: setMemoryDir (unsigned short dir, unsigned char value){ if (dir < CPU_RAM_SIZE){ cpu_memory[dir] = value; } }

unsigned short CPU :: getPC(){ return PC; }

unsigned char CPU :: getA (){ return A; }

unsigned char CPU :: getP (){ return P; }

unsigned short CPU :: getSP(){ return SP; }

unsigned char CPU :: getX (){ return X; }

unsigned char CPU :: getY (){ return Y; }

unsigned char CPU :: getMemoryDir (unsigned short dir){ if (dir < CPU_RAM_SIZE) {return cpu_memory[dir];}}


void CPU :: executeOpcode(OpcodeInfo & info, void* param = nullptr) {

    if (!info.handler.func.void_func) {
        // Handle illegal/undefined opcode
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

int CPU :: emulationCycle(){
    
    instruction_cycles = 0;

    unsigned char opcode = cpu_memory[PC];

    OpcodeInfo info = opcodeTable[opcode];

    unsigned char * arg;
    unsigned short aux;
/*
    printf("ANTES:\n");
    printf("PC: %x --> %d\n", PC, PC);
    printf("Opcode: %x --> %d\n", opcode, opcode);
    printf("A: %x --> %d\n", A, A);
    printf("SP: %x --> %d\n", SP, SP);
    printf("X: %x --> %d\n", X, X);
    printf("Y: %x --> %d\n", Y, Y);
    printf("Flags: %x --> %d\n", P, P);
*/
    switch (info.mode)
    {
    case IMPLIED:
        arg = nullptr;
        break;
    
    case ACCUMULATOR:
        arg = &A;
        break;

    case IMMEDIATE:
        arg = &cpu_memory[PC+1];
        break;
    
    case ZEROPAGE:
        arg = &cpu_memory[cpu_memory[PC+1] % 256];
        break;

    case ZEROPAGE_X:
        arg = &cpu_memory[(cpu_memory[PC+1] + X) % 256];
        break;

    case ZEROPAGE_Y:
        arg = &cpu_memory[(cpu_memory[PC+1] + Y) % 256];
        break;
    
    case ABSOLUTE:
        aux = (cpu_memory[PC+2] << 8) + cpu_memory[PC+1];
        arg = &cpu_memory[aux];
        break;
    
    case ABSOLUTE_X:
        aux = (cpu_memory[PC+2] << 8) + cpu_memory[PC+1];
        arg = &cpu_memory[(aux+X)%CPU_RAM_SIZE];
        
        if ((aux & 0xFF00) != ((aux+X) & 0xFF00) && info.handler.func.uchar_ptr_func != &CPU::ASL && info.handler.func.ushort_func != &CPU::DEC &&
            info.handler.func.ushort_func != &CPU::INC && info.handler.func.uchar_ptr_func != &CPU::LSR && info.handler.func.uchar_ptr_func != &CPU::ROL &&
            info.handler.func.uchar_ptr_func != &CPU::ROR && info.handler.func.ushort_func != &CPU::STA)
            ++instruction_cycles;
        break;
    
    case ABSOLUTE_Y:
        aux = (cpu_memory[PC+2] << 8) + cpu_memory[PC+1];
        arg = &cpu_memory[(aux+Y)%CPU_RAM_SIZE];
        
        if ((aux & 0xFF00) != ((aux+Y) & 0xFF00) && info.handler.func.ushort_func != &CPU::STA)
            ++instruction_cycles;
        break;
    case INDIRECT:
        aux = (cpu_memory[PC+2] << 8) + cpu_memory[PC+1];
        aux = (cpu_memory[aux+1] << 8) + cpu_memory[aux];
        arg = &cpu_memory[(cpu_memory[aux+1]) << 8 + cpu_memory[aux]];
        break;
    case INDIRECT_X:
        arg = &cpu_memory[cpu_memory[(cpu_memory[PC+1] + X) % 256] + cpu_memory[(cpu_memory[PC+1] + X + 1) % 256] * 256];
        break;
    case INDIRECT_Y:
        aux = cpu_memory[PC+1];
        aux = (cpu_memory[(aux+1) % 256] << 8) + cpu_memory[aux % 256];
        arg = &cpu_memory[(aux+Y)%CPU_RAM_SIZE];
        
        if ((aux & 0xFF00) != ((aux+Y) & 0xFF00) && info.handler.func.ushort_func != &CPU::STA)
            ++instruction_cycles;
        break;

    case RELATIVE:
        arg = nullptr;
        break;
    
    default:
        arg = nullptr;
        break;
    }
    
    if (info.handler.type == InstructionHandler::TYPE_USHORT)
        executeOpcode(info, &aux);
    else
        executeOpcode(info, arg);
/*
    printf("DESPUÉS:\n");
    printf("PC: %x --> %d\n", PC, PC);
    printf("Opcode: %x --> %d\n", opcode, opcode);
    printf("A: %x --> %d\n", A, A);
    printf("SP: %x --> %d\n", SP, SP);
    printf("X: %x --> %d\n", X, X);
    printf("Y: %x --> %d\n", Y, Y);
    printf("Flags: %x --> %d\n", P, P);
*/
    if (modifyPC)
        PC += info.length;
    else
        modifyPC = true;

    instruction_cycles += info.cycles;
    return instruction_cycles;
}