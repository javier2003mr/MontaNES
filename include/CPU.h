#define OPCODE_TABLE_SIZE 256
#define CPU_RAM_SIZE 65536

enum AddressingMode {
    IMPLICIT,    // Implícito
    ACCUMULATOR, // Acumulador
    IMMEDIATE,   // Inmediato
    ZEROPAGE,    // Página cero
    ZEROPAGE_X,  // Página cero,X
    ZEROPAGE_Y,  // Página cero,Y
    ABSOLUTE,    // Absoluto
    ABSOLUTE_X,  // Absoluto,X
    ABSOLUTE_Y,  // Absoluto,Y
    INDIRECT,    // Indirecto
    INDIRECT_X,  // (Indirecto,X)
    INDIRECT_Y,  // (Indirecto),Y
    RELATIVE     // Relativo
};

struct OpcodeInfo {
    InstructionHandler handler;
    int length;
    int cycles;
    AddressingMode mode;
};

class CPU {
    private:
        
        unsigned char cpu_memory[CPU_RAM_SIZE];

        // Contador de programa
        unsigned short PC;

        // Acumulador
        unsigned char A;

        // Indices
        unsigned char X, Y;

        // Puntero de pila
        unsigned char SP;

        // Registros de estado
        unsigned char P;

        enum Flags {
            CARRY     = (1 << 0),  // Carry Flag
            ZERO      = (1 << 1),  // Zero Flag
            INTERRUPT = (1 << 2),  // Interrupt Disable
            DECIMAL   = (1 << 3),  // Decimal Mode (no usado en NES)
            BREAK     = (1 << 4),  // Break Command
            OVERFLOW  = (1 << 6),  // Overflow Flag
            NEGATIVE  = (1 << 7)   // Negative Flag
        };

        OpcodeInfo opcodeTable[OPCODE_TABLE_SIZE];

    public:

        // Arithmetic Operations
        void ADC(unsigned char value);
        void SBC(unsigned char value);
        void INC(unsigned short dir);
        void DEC(unsigned short dir);
        void INX();
        void DEX();
        void INY();
        void DEY();

        // Bitwise Operations
        void AND(unsigned char value);
        void ORA(unsigned char value);
        void EOR(unsigned char value);
        void BIT(unsigned char value);

        // Shift Operations
        void ASL(unsigned char * value);
        void LSR(unsigned char * value);
        void ROL(unsigned char * value);
        void ROR(unsigned char * value);
        
        // Compare Operations
        void CMP_GEN(unsigned char value, unsigned char reg);
        void CMP(unsigned char value);
        void CPX(unsigned char value);
        void CPY(unsigned char value);

        // Branch Operations
        void BXX(bool flag);
        void BCC();
        void BCS();
        void BEQ();
        void BNE();
        void BMI();
        void BVC();
        void BVS();
        void BPL();

        // Jump Operations
        void JMP(unsigned short dir);
        void JSR(unsigned short dir);
        void RTS();
        void RTI();
        void BRK(); //<<<<----- NO IMPLEMENTADA AUN

        // Access Operations
        void LDA(unsigned short dir);
        void STA(unsigned short dir);
        void LDX(unsigned short dir);
        void STX(unsigned short dir);
        void LDY(unsigned short dir);
        void STY(unsigned short dir);

        // Transfer Operations
        void TAX();
        void TAY();
        void TXA();
        void TYA();
        
        // Stack Operations
        void PHA();
        void PLA();
        void PHP();
        void PLP();
        void TSX();
        void TXS();

        // Flag Operations
        void CLC();
        void SEC();
        void CLI();
        void SEI();
        void CLD();
        void SED();
        void CLV();

        // Other Operations
        // void NOP();

        void stack_push(unsigned char data);
        unsigned char stack_pop();
        void initializeOpcodeTable();
        bool getFlag(Flags flag);
        void setFlag(Flags flag, bool condition);
        void emulationCycle();
};