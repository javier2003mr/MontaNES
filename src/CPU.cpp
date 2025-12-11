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
    bool overflow = (~(A ^ value) & (A ^ sum)) & 0x80;

    A = static_cast<unsigned char>(sum);

    setFlag(CARRY, sum > 0xFF);
    setFlag(ZERO, A == 0);
    setFlag(OVERFLOW, overflow);
    setFlag(NEGATIVE, A & 0x80);
}

void CPU :: SBC (unsigned char value){
    
    unsigned short result = A + (~value) + ((getFlag(CARRY)) ? 1 : 0);
    bool overflow = ((A ^ result) & (A ^ (~value)) & 0x80) != 0;
    
    A = static_cast<unsigned char>(result);

    setFlag(CARRY, result <= 0xFF);
    setFlag(ZERO, A == 0);
    setFlag(OVERFLOW, overflow);
    setFlag(NEGATIVE, A & 0x80);
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
    
    setFlag(ZERO, A == 0);
    setFlag(NEGATIVE, A & 0x80);
}

void CPU :: ORA (unsigned char value){
    
    A |= value;

    setFlag(ZERO, A == 0);
    setFlag(NEGATIVE, A & 0x80);
}

void CPU :: EOR (unsigned char value){

    A ^= value;

    setFlag(ZERO, A == 0);
    setFlag(NEGATIVE, A & 0x80);
}

void CPU :: BIT (unsigned char value){

    unsigned char aux = A & value;
    
    setFlag(ZERO, aux == 0);
    setFlag(OVERFLOW, value & 0x40);
    setFlag(NEGATIVE, value & 0x80);
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

void CPU :: CMP_GEN (unsigned char value, unsigned char reg){
    setFlag(CARRY, reg >= value);
    setFlag(ZERO, reg == value);
    setFlag(NEGATIVE, (reg - value) & 0x80);
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

void CPU :: JMP (unsigned short dir){
    PC = dir;
}

void CPU :: JSR (unsigned short dir){
    
    stack_push((PC & 0xFF00) >> 8);
    stack_push(PC & 0x00FF);
    PC = dir;
}

void CPU :: RTS (){

    unsigned char low_byte = stack_pop();
    unsigned char high_byte = stack_pop();
    PC = high_byte << 8 | low_byte;
}

void CPU :: RTI (){

    P = stack_pop() & 0xCF;
    RTS();
}

void CPU :: BRK (){
    stack_push(((PC+1) & 0xFF00) >> 8);
    stack_push((PC+1) & 0x00FF);
    stack_push(P | 0x30);
}

void CPU :: TAX (){
    
    X = A;
    setFlag(ZERO, A == 0);
    setFlag(NEGATIVE, A & 0x80);
}

void CPU :: TAY (){

    Y = A;
    setFlag(ZERO, Y == 0);
    setFlag(NEGATIVE, Y & 0x80);
}

void CPU :: TXA (){
    
    A = X;
    setFlag(ZERO, X == 0);
    setFlag(NEGATIVE, X & 0x80);
}

void CPU :: TYA (){
    
    A = Y;
    setFlag(ZERO, A == 0);
    setFlag(NEGATIVE, A & 0x80);
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

void CPU :: PHP (){
    stack_push(P | 0x30);
}

void CPU :: PLP (){
    P |= stack_pop() & 0xCF;
}

void CPU :: LDA (unsigned short dir){
    A = cpu_memory[dir];
    setFlag(ZERO, A == 0);
    setFlag(NEGATIVE, A & 0x80);
}

void CPU :: STA (unsigned short dir){
    cpu_memory[dir] = A;
}

void CPU :: LDX (unsigned short dir){
    X = cpu_memory[dir];
    setFlag(ZERO, X == 0);
    setFlag(NEGATIVE, X & 0x80);
}

void CPU :: STX (unsigned short dir){
    cpu_memory[dir] = X;
}

void CPU :: LDY (unsigned short dir){
    Y = cpu_memory[dir];
    setFlag(ZERO, Y == 0);
    setFlag(NEGATIVE, Y & 0x80);
}

void CPU :: STY (unsigned short dir){
    cpu_memory[dir] = Y;
}

void CPU :: CLC (){
    setFlag(CARRY, 0);
}

void CPU :: SEC (){
    setFlag(CARRY, 1);
}

void CPU :: CLI (){
    setFlag(INTERRUPT, 0);
}

void CPU :: SEI (){
    setFlag(INTERRUPT, 1);
}

void CPU :: CLD (){
    setFlag(DECIMAL, 0);
}

void CPU :: SED (){
    setFlag(DECIMAL, 1);
}

void CPU :: CLV (){
    setFlag(OVERFLOW, 0);
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

}