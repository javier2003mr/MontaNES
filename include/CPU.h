#ifndef CPUCLASS
#define CPUCLASS

#include <functional>
//#include "PPU.hpp"
#include "Joypad.hpp"
#include "APU.hpp"
#include "Cartridge.hpp"

#define OPCODE_TABLE_SIZE 256
#define CPU_RAM_SIZE 65536

enum AddressingMode {
    IMPLIED,    // Implícito
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



class PPU; // Forward declaration

class CPU {
    private:

        struct InstructionHandler {
            enum {
                TYPE_VOID,      // No parameters
                TYPE_UCHAR,     // unsigned char
                TYPE_USHORT,    // unsigned short
                TYPE_UCHAR_PTR, // unsigned char*
                TYPE_BOOL,      // bool
                TYPE_USHORT2,   // unsigned short (for JMP, etc.)
                TYPE_UC_UC      // unsigned char, unsigned char
            } type;
            
            union {
                void (CPU::*void_func)(void);
                void (CPU::*uchar_func)(unsigned char);
                void (CPU::*ushort_func)(unsigned short);
                void (CPU::*uchar_ptr_func)(unsigned char*);
                void (CPU::*bool_func)(bool);
                void (CPU::*uc_uc_func)(unsigned char, unsigned char);
            } func;
        };

        struct OpcodeInfo {
            InstructionHandler handler;
            int length;
            int cycles;
            AddressingMode mode;
        };
        
        unsigned char cpu_memory[CPU_RAM_SIZE];
        PPU* ppu = nullptr;
        Joypad * joypad = nullptr;
        Cartridge * cartridge = nullptr;
        APU* apu = nullptr;

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
            C_FLAG = (1 << 0),  // Carry Flag
            Z_FLAG = (1 << 1),  // Zero Flag
            I_FLAG = (1 << 2),  // Interrupt Disable
            D_FLAG = (1 << 3),  // Decimal Mode (no usado en NES)
            B_FLAG = (1 << 4),  // Break Command
            V_FLAG = (1 << 6),  // Overflow Flag
            N_FLAG = (1 << 7)   // Negative Flag
        };

        // Cuando hay un salto se activa
        bool modifyPC;
        int instruction_cycles;

        OpcodeInfo opcodeTable[OPCODE_TABLE_SIZE];

    public:

        CPU();

        void reset(void);

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
        void BRK();

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
        void NOP();

        // Unofficial Operations
        void STP();
        void SLO(unsigned char * value);
        void ANC(unsigned char value);
        void RLA(unsigned char * value);
        void SRE (unsigned char * value);
        void ASR (unsigned char * value);
        void RRA (unsigned char * value);
        void ARR (unsigned char * value);
        void SAX (unsigned char * value);
        void XAA (unsigned char value);
        void SHX (unsigned short dir);
        void SHY (unsigned short dir);
        void LAX (unsigned short dir);
        void LAS (unsigned char value);
        void DCP (unsigned char * value);
        void SBX (unsigned char value);
        void ISC (unsigned char * value);
        void SHA_absoluteY(unsigned short dir);
        void SHA_indirectY(unsigned char zp_addr);
        void SHA (unsigned short dir);

        void nmi();
        void connectPPU(PPU* ppu_ptr);
        void connectJoypad (Joypad * joypad_ptr);
        void connectCartridge (Cartridge * c);
        void connectAPU (APU * a);

        void setPC(unsigned short value);
        void setA (unsigned char value);
        void setP (unsigned char value);
        void setSP(unsigned short value);
        void setX (unsigned char value);
        void setY (unsigned char value);
        void setMemoryValue (unsigned short dir, unsigned char value);

        unsigned short getPC();
        unsigned char getA ();
        unsigned char getP ();
        unsigned short getSP();
        unsigned char getX ();
        unsigned char getY ();
        unsigned char getMemoryValue (unsigned short dir);

        unsigned char * getMemoryDir(unsigned short dir);

        void stack_push(unsigned char data);
        unsigned char stack_pop();
        void initializeOpcodeTable();
        bool getFlag(Flags flag);
        void setFlag(Flags flag, bool condition);
        void executeOpcode(OpcodeInfo & info, void* param);
        int emulationCycle();
};

#endif