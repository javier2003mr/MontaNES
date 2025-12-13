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

bool CPU :: getFlag(Flags flag) {
    return (P & flag) != 0;
}

void CPU :: setFlag(Flags flag, bool condition) {
    if (condition)
        P |= flag;
    else
        P &= ~flag;
}

void CPU :: executeOpcode(OpcodeInfo & info, void* param1 = nullptr, void* param2 = nullptr) {

    if (!info.handler.func.void_func) {
        // Handle illegal/undefined opcode
        return;
    }
    
    switch (info.handler.type) {
        case InstructionHandler::TYPE_VOID:
            (this->*(info.handler.func.void_func))();
            break;
            
        case InstructionHandler::TYPE_UCHAR:
            (this->*(info.handler.func.uchar_func))(*static_cast<unsigned char*>(param1));
            break;
            
        case InstructionHandler::TYPE_USHORT:
            (this->*(info.handler.func.ushort_func))(*static_cast<unsigned short*>(param1));
            break;
            
        case InstructionHandler::TYPE_UCHAR_PTR:
            (this->*(info.handler.func.uchar_ptr_func))(static_cast<unsigned char*>(param1));
            break;
            
        case InstructionHandler::TYPE_BOOL:
            (this->*(info.handler.func.bool_func))(*static_cast<bool*>(param1));
            break;
            
        case InstructionHandler::TYPE_UC_UC:
            (this->*(info.handler.func.uc_uc_func))(
                *static_cast<unsigned char*>(param1),
                *static_cast<unsigned char*>(param2)
            );
            break;
    }
}

void CPU :: emulationCycle(){
    unsigned char opcode = cpu_memory[PC];

    OpcodeInfo info = opcodeTable[opcode];

    switch (info.mode)
    {
    case IMPLIED:
        break;
    
    case ACCUMULATOR:
        break;

    case IMMEDIATE:
        break;
    
    case ZEROPAGE:
        break;

    case ZEROPAGE_X:
        break;

    case ZEROPAGE_Y:
        break;
    
    case ABSOLUTE:
        break;
    
    case ABSOLUTE_X:
        break;

    case INDIRECT:
        break;
    
    case INDIRECT_X:
        break;

    case INDIRECT_Y:
        break;

    case RELATIVE:
        break;
    
    default:
        break;
    }
}