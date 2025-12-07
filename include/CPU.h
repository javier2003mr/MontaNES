#define OPCODE_TABLE_SIZE

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
    uint8_t cycles;
    AddressingMode mode;
};

class CPU {
    private:
        
        // Contador de programa
        unsigned short PC;

        // Acumulador
        unsigned char A;

        // Indices
        unsigned char X, Y;

        // Puntero de pila
        unsigned char SP;

        // Registros de estado
        // Mapa de bits:
        // 0: Carry
        // 1: Zero
        // 2: Interrupt Disable
        // 3: Decimal
        // 4: B flag
        // 5: 1
        // 6: Overflow
        // 7: Negative
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
        void AND(unsigned char value);

        // Shift Operations
        void ASL(unsigned char * value);
        void LSR(unsigned char * value);
        void ROL(unsigned char * value);
        void ROR(unsigned char * value);
        
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

        void initializeOpcodeTable();
        bool getFlag(Flags flag);
        void setFlag(Flags flag, bool condition);
        void emulationCycle();
};