#include "../include/CPU.h"

void CPU :: initializeOpcodeTable() {
    // Initialize all opcodes as NOP first
    for (int i = 0; i < 256; i++) {
        opcodeTable[i].handler.type = InstructionHandler::TYPE_VOID;
        opcodeTable[i].handler.func.void_func = &CPU::NOP;
        opcodeTable[i].length = 1;
        opcodeTable[i].cycles = 2;
        opcodeTable[i].mode = IMPLIED;
    }
    
    // Opcode 0x00: BRK
    opcodeTable[0x00].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x00].handler.func.void_func = &CPU::BRK;
    opcodeTable[0x00].length = 1;
    opcodeTable[0x00].cycles = 7;
    opcodeTable[0x00].mode = IMPLIED;
    
    // Opcode 0x01: ORA (Indirect,X)
    opcodeTable[0x01].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x01].handler.func.uchar_func = &CPU::ORA;
    opcodeTable[0x01].length = 2;
    opcodeTable[0x01].cycles = 6;
    opcodeTable[0x01].mode = INDIRECT_X;
    
    // Opcode 0x02: Undefined (STP)
    opcodeTable[0x02].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x02].handler.func.void_func = &CPU::STP;
    opcodeTable[0x02].length = 1;
    opcodeTable[0x02].cycles = 3;
    opcodeTable[0x02].mode = IMPLIED;
    
    // Opcode 0x03: Undefined
    opcodeTable[0x03].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x03].handler.func.uchar_ptr_func = &CPU::SLO;
    opcodeTable[0x03].length = 2;
    opcodeTable[0x03].cycles = 8;
    opcodeTable[0x03].mode = INDIRECT_X;

    // Opcode 0x04: Undefined
    opcodeTable[0x04].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x04].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x04].length = 2;
    opcodeTable[0x04].cycles = 3;
    opcodeTable[0x04].mode = ZEROPAGE;

    // Opcode 0x05: ORA Zero Page
    opcodeTable[0x05].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x05].handler.func.uchar_func = &CPU::ORA;
    opcodeTable[0x05].length = 2;
    opcodeTable[0x05].cycles = 3;
    opcodeTable[0x05].mode = ZEROPAGE;
    
    // Opcode 0x06: ASL Zero Page
    opcodeTable[0x06].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x06].handler.func.uchar_ptr_func = &CPU::ASL;
    opcodeTable[0x06].length = 2;
    opcodeTable[0x06].cycles = 5;
    opcodeTable[0x06].mode = ZEROPAGE;
    
    // Opcode 0x07: Undefined
    opcodeTable[0x07].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x07].handler.func.uchar_ptr_func = &CPU::SLO;
    opcodeTable[0x07].length = 2;
    opcodeTable[0x07].cycles = 5;
    opcodeTable[0x07].mode = ZEROPAGE;

    // Opcode 0x08: PHP
    opcodeTable[0x08].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x08].handler.func.void_func = &CPU::PHP;
    opcodeTable[0x08].length = 1;
    opcodeTable[0x08].cycles = 3;
    opcodeTable[0x08].mode = IMPLIED;
    
    // Opcode 0x09: ORA Immediate
    opcodeTable[0x09].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x09].handler.func.uchar_func = &CPU::ORA;
    opcodeTable[0x09].length = 2;
    opcodeTable[0x09].cycles = 2;
    opcodeTable[0x09].mode = IMMEDIATE;
    
    // Opcode 0x0A: ASL Accumulator
    opcodeTable[0x0A].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x0A].handler.func.uchar_ptr_func = &CPU::ASL;
    opcodeTable[0x0A].length = 1;
    opcodeTable[0x0A].cycles = 2;
    opcodeTable[0x0A].mode = ACCUMULATOR;
    
    // Opcode 0x0B: Undefined
    opcodeTable[0x0B].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x0B].handler.func.uchar_func = &CPU::ANC;
    opcodeTable[0x0B].length = 2;
    opcodeTable[0x0B].cycles = 2;
    opcodeTable[0x0B].mode = IMMEDIATE;

    // Opcode 0x0C: Undefined
    opcodeTable[0x0C].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x0C].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x0C].length = 3;
    opcodeTable[0x0C].cycles = 4;
    opcodeTable[0x0C].mode = ABSOLUTE;

    // Opcode 0x0D: ORA Absolute
    opcodeTable[0x0D].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x0D].handler.func.uchar_func = &CPU::ORA;
    opcodeTable[0x0D].length = 3;
    opcodeTable[0x0D].cycles = 4;
    opcodeTable[0x0D].mode = ABSOLUTE;
    
    // Opcode 0x0E: ASL Absolute
    opcodeTable[0x0E].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x0E].handler.func.uchar_ptr_func = &CPU::ASL;
    opcodeTable[0x0E].length = 3;
    opcodeTable[0x0E].cycles = 6;
    opcodeTable[0x0E].mode = ABSOLUTE;
    
    // Opcode 0x0F: Undefined
    opcodeTable[0x0F].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x0F].handler.func.uchar_ptr_func = &CPU::SLO;
    opcodeTable[0x0F].length = 3;
    opcodeTable[0x0F].cycles = 6;
    opcodeTable[0x0F].mode = ABSOLUTE;

    // Opcode 0x10: BPL
    opcodeTable[0x10].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x10].handler.func.void_func = &CPU::BPL;
    opcodeTable[0x10].length = 2;
    opcodeTable[0x10].cycles = 2;
    opcodeTable[0x10].mode = RELATIVE;
    
    // Opcode 0x11: ORA (Indirect),Y
    opcodeTable[0x11].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x11].handler.func.uchar_func = &CPU::ORA;
    opcodeTable[0x11].length = 2;
    opcodeTable[0x11].cycles = 5;
    opcodeTable[0x11].mode = INDIRECT_Y;
    
    // Opcode 0x12: Undefined
    opcodeTable[0x12].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x12].handler.func.void_func = &CPU::STP;
    opcodeTable[0x12].length = 1;
    opcodeTable[0x12].cycles = 3;
    opcodeTable[0x12].mode = IMPLIED;

    // Opcode 0x13: Undefined
    opcodeTable[0x13].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x13].handler.func.uchar_ptr_func = &CPU::SLO;
    opcodeTable[0x13].length = 2;
    opcodeTable[0x13].cycles = 8;
    opcodeTable[0x13].mode = INDIRECT_Y;

    // Opcode 0x14: Undefined
    opcodeTable[0x14].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x14].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x14].length = 2;
    opcodeTable[0x14].cycles = 4;
    opcodeTable[0x14].mode = ZEROPAGE_X;

    // Opcode 0x15: ORA Zero Page,X
    opcodeTable[0x15].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x15].handler.func.uchar_func = &CPU::ORA;
    opcodeTable[0x15].length = 2;
    opcodeTable[0x15].cycles = 4;
    opcodeTable[0x15].mode = ZEROPAGE_X;
    
    // Opcode 0x16: ASL Zero Page,X
    opcodeTable[0x16].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x16].handler.func.uchar_ptr_func = &CPU::ASL;
    opcodeTable[0x16].length = 2;
    opcodeTable[0x16].cycles = 6;
    opcodeTable[0x16].mode = ZEROPAGE_X;
    
    // Opcode 0x17: Undefined
    opcodeTable[0x17].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x17].handler.func.uchar_ptr_func = &CPU::SLO;
    opcodeTable[0x17].length = 2;
    opcodeTable[0x17].cycles = 6;
    opcodeTable[0x17].mode = ZEROPAGE_X;

    // Opcode 0x18: CLC
    opcodeTable[0x18].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x18].handler.func.void_func = &CPU::CLC;
    opcodeTable[0x18].length = 1;
    opcodeTable[0x18].cycles = 2;
    opcodeTable[0x18].mode = IMPLIED;
    
    // Opcode 0x19: ORA Absolute,Y
    opcodeTable[0x19].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x19].handler.func.uchar_func = &CPU::ORA;
    opcodeTable[0x19].length = 3;
    opcodeTable[0x19].cycles = 4;
    opcodeTable[0x19].mode = ABSOLUTE_Y;
    
    // Opcode 0x1A: Undefined
    
    // Opcode 0x1B: Undefined
    opcodeTable[0x1B].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x1B].handler.func.uchar_ptr_func = &CPU::SLO;
    opcodeTable[0x1B].length = 3;
    opcodeTable[0x1B].cycles = 7;
    opcodeTable[0x1B].mode = ABSOLUTE_Y;

    // Opcode 0x1C: Undefined
    opcodeTable[0x1C].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x1C].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x1C].length = 3;
    opcodeTable[0x1C].cycles = 4;
    opcodeTable[0x1C].mode = ABSOLUTE_X;

    // Opcode 0x1D: ORA Absolute,X
    opcodeTable[0x1D].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x1D].handler.func.uchar_func = &CPU::ORA;
    opcodeTable[0x1D].length = 3;
    opcodeTable[0x1D].cycles = 4;
    opcodeTable[0x1D].mode = ABSOLUTE_X;
    
    // Opcode 0x1E: ASL Absolute,X
    opcodeTable[0x1E].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x1E].handler.func.uchar_ptr_func = &CPU::ASL;
    opcodeTable[0x1E].length = 3;
    opcodeTable[0x1E].cycles = 7;
    opcodeTable[0x1E].mode = ABSOLUTE_X;
    
    // Opcode 0x1F: Undefined
    opcodeTable[0x1F].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x1F].handler.func.uchar_ptr_func = &CPU::SLO;
    opcodeTable[0x1F].length = 3;
    opcodeTable[0x1F].cycles = 7;
    opcodeTable[0x1F].mode = ABSOLUTE_X;

    // Opcode 0x20: JSR
    opcodeTable[0x20].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x20].handler.func.ushort_func = &CPU::JSR;
    opcodeTable[0x20].length = 3;
    opcodeTable[0x20].cycles = 6;
    opcodeTable[0x20].mode = ABSOLUTE;
    
    // Opcode 0x21: AND (Indirect,X)
    opcodeTable[0x21].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x21].handler.func.uchar_func = &CPU::AND;
    opcodeTable[0x21].length = 2;
    opcodeTable[0x21].cycles = 6;
    opcodeTable[0x21].mode = INDIRECT_X;
    
    // Opcode 0x22: Undefined
    opcodeTable[0x22].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x22].handler.func.void_func = &CPU::STP;
    opcodeTable[0x22].length = 1;
    opcodeTable[0x22].cycles = 3;
    opcodeTable[0x22].mode = IMPLIED;

    // Opcode 0x23: Undefined
    opcodeTable[0x23].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x23].handler.func.uchar_ptr_func = &CPU::RLA;
    opcodeTable[0x23].length = 2;
    opcodeTable[0x23].cycles = 8;
    opcodeTable[0x23].mode = INDIRECT_X;

    // Opcode 0x24: BIT Zero Page
    opcodeTable[0x24].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x24].handler.func.uchar_func = &CPU::BIT;
    opcodeTable[0x24].length = 2;
    opcodeTable[0x24].cycles = 3;
    opcodeTable[0x24].mode = ZEROPAGE;
    
    // Opcode 0x25: AND Zero Page
    opcodeTable[0x25].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x25].handler.func.uchar_func = &CPU::AND;
    opcodeTable[0x25].length = 2;
    opcodeTable[0x25].cycles = 3;
    opcodeTable[0x25].mode = ZEROPAGE;
    
    // Opcode 0x26: ROL Zero Page
    opcodeTable[0x26].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x26].handler.func.uchar_ptr_func = &CPU::ROL;
    opcodeTable[0x26].length = 2;
    opcodeTable[0x26].cycles = 5;
    opcodeTable[0x26].mode = ZEROPAGE;
    
    // Opcode 0x27: Undefined
    opcodeTable[0x27].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x27].handler.func.uchar_ptr_func = &CPU::RLA;
    opcodeTable[0x27].length = 2;
    opcodeTable[0x27].cycles = 5;
    opcodeTable[0x27].mode = ZEROPAGE;

    // Opcode 0x28: PLP
    opcodeTable[0x28].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x28].handler.func.void_func = &CPU::PLP;
    opcodeTable[0x28].length = 1;
    opcodeTable[0x28].cycles = 4;
    opcodeTable[0x28].mode = IMPLIED;
    
    // Opcode 0x29: AND Immediate
    opcodeTable[0x29].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x29].handler.func.uchar_func = &CPU::AND;
    opcodeTable[0x29].length = 2;
    opcodeTable[0x29].cycles = 2;
    opcodeTable[0x29].mode = IMMEDIATE;
    
    // Opcode 0x2A: ROL Accumulator
    opcodeTable[0x2A].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x2A].handler.func.uchar_ptr_func = &CPU::ROL;
    opcodeTable[0x2A].length = 1;
    opcodeTable[0x2A].cycles = 2;
    opcodeTable[0x2A].mode = ACCUMULATOR;
    
    // Opcode 0x2B: Undefined
    opcodeTable[0x2B].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x2B].handler.func.uchar_func = &CPU::ANC;
    opcodeTable[0x2B].length = 2;
    opcodeTable[0x2B].cycles = 2;
    opcodeTable[0x2B].mode = IMMEDIATE;

    // Opcode 0x2C: BIT Absolute
    opcodeTable[0x2C].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x2C].handler.func.uchar_func = &CPU::BIT;
    opcodeTable[0x2C].length = 3;
    opcodeTable[0x2C].cycles = 4;
    opcodeTable[0x2C].mode = ABSOLUTE;
    
    // Opcode 0x2D: AND Absolute
    opcodeTable[0x2D].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x2D].handler.func.uchar_func = &CPU::AND;
    opcodeTable[0x2D].length = 3;
    opcodeTable[0x2D].cycles = 4;
    opcodeTable[0x2D].mode = ABSOLUTE;
    
    // Opcode 0x2E: ROL Absolute
    opcodeTable[0x2E].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x2E].handler.func.uchar_ptr_func = &CPU::ROL;
    opcodeTable[0x2E].length = 3;
    opcodeTable[0x2E].cycles = 6;
    opcodeTable[0x2E].mode = ABSOLUTE;
    
    // Opcode 0x2F: Undefined
    opcodeTable[0x2F].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x2F].handler.func.uchar_ptr_func = &CPU::RLA;
    opcodeTable[0x2F].length = 3;
    opcodeTable[0x2F].cycles = 6;
    opcodeTable[0x2F].mode = ABSOLUTE;

    // Opcode 0x30: BMI
    opcodeTable[0x30].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x30].handler.func.void_func = &CPU::BMI;
    opcodeTable[0x30].length = 2;
    opcodeTable[0x30].cycles = 2;
    opcodeTable[0x30].mode = RELATIVE;
    
    // Opcode 0x31: AND (Indirect),Y
    opcodeTable[0x31].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x31].handler.func.uchar_func = &CPU::AND;
    opcodeTable[0x31].length = 2;
    opcodeTable[0x31].cycles = 5;
    opcodeTable[0x31].mode = INDIRECT_Y;
    
    // Opcode 0x32: Undefined
    opcodeTable[0x32].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x32].handler.func.void_func = &CPU::STP;
    opcodeTable[0x32].length = 1;
    opcodeTable[0x32].cycles = 3;
    opcodeTable[0x32].mode = IMPLIED;

    // Opcode 0x33: Undefined
    opcodeTable[0x33].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x33].handler.func.uchar_ptr_func = &CPU::RLA;
    opcodeTable[0x33].length = 2;
    opcodeTable[0x33].cycles = 8;
    opcodeTable[0x33].mode = INDIRECT_Y;

    // Opcode 0x34: Undefined
    opcodeTable[0x34].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x34].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x34].length = 2;
    opcodeTable[0x34].cycles = 4;
    opcodeTable[0x34].mode = ZEROPAGE_X;

    // Opcode 0x35: AND Zero Page,X
    opcodeTable[0x35].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x35].handler.func.uchar_func = &CPU::AND;
    opcodeTable[0x35].length = 2;
    opcodeTable[0x35].cycles = 4;
    opcodeTable[0x35].mode = ZEROPAGE_X;
    
    // Opcode 0x36: ROL Zero Page,X
    opcodeTable[0x36].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x36].handler.func.uchar_ptr_func = &CPU::ROL;
    opcodeTable[0x36].length = 2;
    opcodeTable[0x36].cycles = 6;
    opcodeTable[0x36].mode = ZEROPAGE_X;
    
    // Opcode 0x37: Undefined
    opcodeTable[0x37].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x37].handler.func.uchar_ptr_func = &CPU::RLA;
    opcodeTable[0x37].length = 2;
    opcodeTable[0x37].cycles = 6;
    opcodeTable[0x37].mode = ZEROPAGE_X;

    // Opcode 0x38: SEC
    opcodeTable[0x38].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x38].handler.func.void_func = &CPU::SEC;
    opcodeTable[0x38].length = 1;
    opcodeTable[0x38].cycles = 2;
    opcodeTable[0x38].mode = IMPLIED;
    
    // Opcode 0x39: AND Absolute,Y
    opcodeTable[0x39].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x39].handler.func.uchar_func = &CPU::AND;
    opcodeTable[0x39].length = 3;
    opcodeTable[0x39].cycles = 4;
    opcodeTable[0x39].mode = ABSOLUTE_Y;
    
    // Opcode 0x3A: Undefined
    
    // Opcode 0x3B: Undefined
    opcodeTable[0x3B].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x3B].handler.func.uchar_ptr_func = &CPU::RLA;
    opcodeTable[0x3B].length = 3;
    opcodeTable[0x3B].cycles = 7;
    opcodeTable[0x3B].mode = ABSOLUTE_Y;

    // Opcode 0x3C: Undefined
    opcodeTable[0x3C].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x3C].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x3C].length = 3;
    opcodeTable[0x3C].cycles = 4;
    opcodeTable[0x3C].mode = ABSOLUTE_X;

    // Opcode 0x3D: AND Absolute,X
    opcodeTable[0x3D].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x3D].handler.func.uchar_func = &CPU::AND;
    opcodeTable[0x3D].length = 3;
    opcodeTable[0x3D].cycles = 4;
    opcodeTable[0x3D].mode = ABSOLUTE_X;
    
    // Opcode 0x3E: ROL Absolute,X
    opcodeTable[0x3E].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x3E].handler.func.uchar_ptr_func = &CPU::ROL;
    opcodeTable[0x3E].length = 3;
    opcodeTable[0x3E].cycles = 7;
    opcodeTable[0x3E].mode = ABSOLUTE_X;
    
    // Opcode 0x3F: Undefined
    opcodeTable[0x3F].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x3F].handler.func.uchar_ptr_func = &CPU::RLA;
    opcodeTable[0x3F].length = 3;
    opcodeTable[0x3F].cycles = 7;
    opcodeTable[0x3F].mode = ABSOLUTE_X;

    // Opcode 0x40: RTI
    opcodeTable[0x40].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x40].handler.func.void_func = &CPU::RTI;
    opcodeTable[0x40].length = 1;
    opcodeTable[0x40].cycles = 6;
    opcodeTable[0x40].mode = IMPLIED;
    
    // Opcode 0x41: EOR (Indirect,X)
    opcodeTable[0x41].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x41].handler.func.uchar_func = &CPU::EOR;
    opcodeTable[0x41].length = 2;
    opcodeTable[0x41].cycles = 6;
    opcodeTable[0x41].mode = INDIRECT_X;
    
    // Opcode 0x42: Undefined
    opcodeTable[0x42].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x42].handler.func.void_func = &CPU::STP;
    opcodeTable[0x42].length = 1;
    opcodeTable[0x42].cycles = 3;
    opcodeTable[0x42].mode = IMPLIED;

    // Opcode 0x43: Undefined
    opcodeTable[0x43].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x43].handler.func.uchar_ptr_func = &CPU::SRE;
    opcodeTable[0x43].length = 2;
    opcodeTable[0x43].cycles = 8;
    opcodeTable[0x43].mode = INDIRECT_X;

    // Opcode 0x44: Undefined
    opcodeTable[0x44].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x44].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x44].length = 2;
    opcodeTable[0x44].cycles = 3;
    opcodeTable[0x44].mode = ZEROPAGE;

    // Opcode 0x45: EOR Zero Page
    opcodeTable[0x45].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x45].handler.func.uchar_func = &CPU::EOR;
    opcodeTable[0x45].length = 2;
    opcodeTable[0x45].cycles = 3;
    opcodeTable[0x45].mode = ZEROPAGE;
    
    // Opcode 0x46: LSR Zero Page
    opcodeTable[0x46].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x46].handler.func.uchar_ptr_func = &CPU::LSR;
    opcodeTable[0x46].length = 2;
    opcodeTable[0x46].cycles = 5;
    opcodeTable[0x46].mode = ZEROPAGE;
    
    // Opcode 0x47: Undefined
    opcodeTable[0x47].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x47].handler.func.uchar_ptr_func = &CPU::SRE;
    opcodeTable[0x47].length = 2;
    opcodeTable[0x47].cycles = 5;
    opcodeTable[0x47].mode = ZEROPAGE;

    // Opcode 0x48: PHA
    opcodeTable[0x48].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x48].handler.func.void_func = &CPU::PHA;
    opcodeTable[0x48].length = 1;
    opcodeTable[0x48].cycles = 3;
    opcodeTable[0x48].mode = IMPLIED;
    
    // Opcode 0x49: EOR Immediate
    opcodeTable[0x49].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x49].handler.func.uchar_func = &CPU::EOR;
    opcodeTable[0x49].length = 2;
    opcodeTable[0x49].cycles = 2;
    opcodeTable[0x49].mode = IMMEDIATE;
    
    // Opcode 0x4A: LSR Accumulator
    opcodeTable[0x4A].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x4A].handler.func.uchar_ptr_func = &CPU::LSR;
    opcodeTable[0x4A].length = 1;
    opcodeTable[0x4A].cycles = 2;
    opcodeTable[0x4A].mode = ACCUMULATOR;
    
    // Opcode 0x4B: Undefined
    opcodeTable[0x4B].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x4B].handler.func.uchar_ptr_func = &CPU::ASR;
    opcodeTable[0x4B].length = 2;
    opcodeTable[0x4B].cycles = 2;
    opcodeTable[0x4B].mode = IMMEDIATE;

    // Opcode 0x4C: JMP Absolute
    opcodeTable[0x4C].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x4C].handler.func.ushort_func = &CPU::JMP;
    opcodeTable[0x4C].length = 3;
    opcodeTable[0x4C].cycles = 3;
    opcodeTable[0x4C].mode = ABSOLUTE;
    
    // Opcode 0x4D: EOR Absolute
    opcodeTable[0x4D].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x4D].handler.func.uchar_func = &CPU::EOR;
    opcodeTable[0x4D].length = 3;
    opcodeTable[0x4D].cycles = 4;
    opcodeTable[0x4D].mode = ABSOLUTE;
    
    // Opcode 0x4E: LSR Absolute
    opcodeTable[0x4E].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x4E].handler.func.uchar_ptr_func = &CPU::LSR;
    opcodeTable[0x4E].length = 3;
    opcodeTable[0x4E].cycles = 6;
    opcodeTable[0x4E].mode = ABSOLUTE;
    
    // Opcode 0x4F: Undefined
    opcodeTable[0x4F].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x4F].handler.func.uchar_ptr_func = &CPU::SRE;
    opcodeTable[0x4F].length = 3;
    opcodeTable[0x4F].cycles = 6;
    opcodeTable[0x4F].mode = ABSOLUTE;

    // Opcode 0x50: BVC
    opcodeTable[0x50].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x50].handler.func.void_func = &CPU::BVC;
    opcodeTable[0x50].length = 2;
    opcodeTable[0x50].cycles = 2;
    opcodeTable[0x50].mode = RELATIVE;
    
    // Opcode 0x51: EOR (Indirect),Y
    opcodeTable[0x51].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x51].handler.func.uchar_func = &CPU::EOR;
    opcodeTable[0x51].length = 2;
    opcodeTable[0x51].cycles = 5;
    opcodeTable[0x51].mode = INDIRECT_Y;
    
    // Opcode 0x52: Undefined
    opcodeTable[0x52].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x52].handler.func.void_func = &CPU::STP;
    opcodeTable[0x52].length = 1;
    opcodeTable[0x52].cycles = 3;
    opcodeTable[0x52].mode = IMPLIED;

    // Opcode 0x53: Undefined
    opcodeTable[0x53].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x53].handler.func.uchar_ptr_func = &CPU::SRE;
    opcodeTable[0x53].length = 2;
    opcodeTable[0x53].cycles = 8;
    opcodeTable[0x53].mode = INDIRECT_Y;

    // Opcode 0x54: Undefined
    opcodeTable[0x54].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x54].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x54].length = 2;
    opcodeTable[0x54].cycles = 4;
    opcodeTable[0x54].mode = ZEROPAGE_X;

    // Opcode 0x55: EOR Zero Page,X
    opcodeTable[0x55].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x55].handler.func.uchar_func = &CPU::EOR;
    opcodeTable[0x55].length = 2;
    opcodeTable[0x55].cycles = 4;
    opcodeTable[0x55].mode = ZEROPAGE_X;
    
    // Opcode 0x56: LSR Zero Page,X
    opcodeTable[0x56].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x56].handler.func.uchar_ptr_func = &CPU::LSR;
    opcodeTable[0x56].length = 2;
    opcodeTable[0x56].cycles = 6;
    opcodeTable[0x56].mode = ZEROPAGE_X;
    
    // Opcode 0x57: Undefined
    opcodeTable[0x57].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x57].handler.func.uchar_ptr_func = &CPU::SRE;
    opcodeTable[0x57].length = 2;
    opcodeTable[0x57].cycles = 6;
    opcodeTable[0x57].mode = ZEROPAGE_X;

    // Opcode 0x58: CLI
    opcodeTable[0x58].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x58].handler.func.void_func = &CPU::CLI;
    opcodeTable[0x58].length = 1;
    opcodeTable[0x58].cycles = 2;
    opcodeTable[0x58].mode = IMPLIED;
    
    // Opcode 0x59: EOR Absolute,Y
    opcodeTable[0x59].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x59].handler.func.uchar_func = &CPU::EOR;
    opcodeTable[0x59].length = 3;
    opcodeTable[0x59].cycles = 4;
    opcodeTable[0x59].mode = ABSOLUTE_Y;
    
    // Opcode 0x5A: Undefined
    
    // Opcode 0x5B: Undefined
    opcodeTable[0x5B].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x5B].handler.func.uchar_ptr_func = &CPU::SRE;
    opcodeTable[0x5B].length = 3;
    opcodeTable[0x5B].cycles = 7;
    opcodeTable[0x5B].mode = ABSOLUTE_Y;

    // Opcode 0x5C: Undefined
    opcodeTable[0x5C].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x5C].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x5C].length = 3;
    opcodeTable[0x5C].cycles = 4;
    opcodeTable[0x5C].mode = ABSOLUTE_X;

    // Opcode 0x5D: EOR Absolute,X
    opcodeTable[0x5D].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x5D].handler.func.uchar_func = &CPU::EOR;
    opcodeTable[0x5D].length = 3;
    opcodeTable[0x5D].cycles = 4;
    opcodeTable[0x5D].mode = ABSOLUTE_X;
    
    // Opcode 0x5E: LSR Absolute,X
    opcodeTable[0x5E].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x5E].handler.func.uchar_ptr_func = &CPU::LSR;
    opcodeTable[0x5E].length = 3;
    opcodeTable[0x5E].cycles = 7;
    opcodeTable[0x5E].mode = ABSOLUTE_X;
    
    // Opcode 0x5F: Undefined
    opcodeTable[0x5F].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x5F].handler.func.uchar_ptr_func = &CPU::SRE;
    opcodeTable[0x5F].length = 3;
    opcodeTable[0x5F].cycles = 7;
    opcodeTable[0x5F].mode = ABSOLUTE_X;

    // Opcode 0x60: RTS
    opcodeTable[0x60].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x60].handler.func.void_func = &CPU::RTS;
    opcodeTable[0x60].length = 1;
    opcodeTable[0x60].cycles = 6;
    opcodeTable[0x60].mode = IMPLIED;
    
    // Opcode 0x61: ADC (Indirect,X)
    opcodeTable[0x61].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x61].handler.func.uchar_func = &CPU::ADC;
    opcodeTable[0x61].length = 2;
    opcodeTable[0x61].cycles = 6;
    opcodeTable[0x61].mode = INDIRECT_X;
    
    // Opcode 0x62: Undefined
    opcodeTable[0x62].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x62].handler.func.void_func = &CPU::STP;
    opcodeTable[0x62].length = 1;
    opcodeTable[0x62].cycles = 3;
    opcodeTable[0x62].mode = IMPLIED;

    // Opcode 0x63: Undefined
    opcodeTable[0x63].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x63].handler.func.uchar_ptr_func = &CPU::RRA;
    opcodeTable[0x63].length = 2;
    opcodeTable[0x63].cycles = 8;
    opcodeTable[0x63].mode = INDIRECT_X;

    // Opcode 0x64: Undefined
    opcodeTable[0x64].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x64].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x64].length = 2;
    opcodeTable[0x64].cycles = 3;
    opcodeTable[0x64].mode = ZEROPAGE;

    // Opcode 0x65: ADC Zero Page
    opcodeTable[0x65].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x65].handler.func.uchar_func = &CPU::ADC;
    opcodeTable[0x65].length = 2;
    opcodeTable[0x65].cycles = 3;
    opcodeTable[0x65].mode = ZEROPAGE;
    
    // Opcode 0x66: ROR Zero Page
    opcodeTable[0x66].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x66].handler.func.uchar_ptr_func = &CPU::ROR;
    opcodeTable[0x66].length = 2;
    opcodeTable[0x66].cycles = 5;
    opcodeTable[0x66].mode = ZEROPAGE;
    
    // Opcode 0x67: Undefined
    opcodeTable[0x67].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x67].handler.func.uchar_ptr_func = &CPU::RRA;
    opcodeTable[0x67].length = 2;
    opcodeTable[0x67].cycles = 5;
    opcodeTable[0x67].mode = ZEROPAGE;

    // Opcode 0x68: PLA
    opcodeTable[0x68].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x68].handler.func.void_func = &CPU::PLA;
    opcodeTable[0x68].length = 1;
    opcodeTable[0x68].cycles = 4;
    opcodeTable[0x68].mode = IMPLIED;
    
    // Opcode 0x69: ADC Immediate
    opcodeTable[0x69].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x69].handler.func.uchar_func = &CPU::ADC;
    opcodeTable[0x69].length = 2;
    opcodeTable[0x69].cycles = 2;
    opcodeTable[0x69].mode = IMMEDIATE;
    
    // Opcode 0x6A: ROR Accumulator
    opcodeTable[0x6A].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x6A].handler.func.uchar_ptr_func = &CPU::ROR;
    opcodeTable[0x6A].length = 1;
    opcodeTable[0x6A].cycles = 2;
    opcodeTable[0x6A].mode = ACCUMULATOR;
    
    // Opcode 0x6B: Undefined
    opcodeTable[0x6B].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x6B].handler.func.uchar_ptr_func = &CPU::ARR;
    opcodeTable[0x6B].length = 2;
    opcodeTable[0x6B].cycles = 2;
    opcodeTable[0x6B].mode = IMMEDIATE;

    // Opcode 0x6C: JMP Indirect
    opcodeTable[0x6C].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x6C].handler.func.ushort_func = &CPU::JMP;
    opcodeTable[0x6C].length = 3;
    opcodeTable[0x6C].cycles = 5;
    opcodeTable[0x6C].mode = INDIRECT;
    
    // Opcode 0x6D: ADC Absolute
    opcodeTable[0x6D].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x6D].handler.func.uchar_func = &CPU::ADC;
    opcodeTable[0x6D].length = 3;
    opcodeTable[0x6D].cycles = 4;
    opcodeTable[0x6D].mode = ABSOLUTE;
    
    // Opcode 0x6E: ROR Absolute
    opcodeTable[0x6E].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x6E].handler.func.uchar_ptr_func = &CPU::ROR;
    opcodeTable[0x6E].length = 3;
    opcodeTable[0x6E].cycles = 6;
    opcodeTable[0x6E].mode = ABSOLUTE;
    
    // Opcode 0x6F: Undefined
    opcodeTable[0x6F].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x6F].handler.func.uchar_ptr_func = &CPU::RRA;
    opcodeTable[0x6F].length = 3;
    opcodeTable[0x6F].cycles = 6;
    opcodeTable[0x6F].mode = ABSOLUTE;

    // Opcode 0x70: BVS
    opcodeTable[0x70].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x70].handler.func.void_func = &CPU::BVS;
    opcodeTable[0x70].length = 2;
    opcodeTable[0x70].cycles = 2;
    opcodeTable[0x70].mode = RELATIVE;
    
    // Opcode 0x71: ADC (Indirect),Y
    opcodeTable[0x71].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x71].handler.func.uchar_func = &CPU::ADC;
    opcodeTable[0x71].length = 2;
    opcodeTable[0x71].cycles = 5;
    opcodeTable[0x71].mode = INDIRECT_Y;
    
    // Opcode 0x72: Undefined
    opcodeTable[0x72].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x72].handler.func.void_func = &CPU::STP;
    opcodeTable[0x72].length = 1;
    opcodeTable[0x72].cycles = 3;
    opcodeTable[0x72].mode = IMPLIED;

    // Opcode 0x73: Undefined
    opcodeTable[0x73].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x73].handler.func.uchar_ptr_func = &CPU::RRA;
    opcodeTable[0x73].length = 2;
    opcodeTable[0x73].cycles = 8;
    opcodeTable[0x73].mode = INDIRECT_Y;

    // Opcode 0x74: Undefined
    opcodeTable[0x74].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x74].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x74].length = 2;
    opcodeTable[0x74].cycles = 4;
    opcodeTable[0x74].mode = ZEROPAGE_X;

    // Opcode 0x75: ADC Zero Page,X
    opcodeTable[0x75].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x75].handler.func.uchar_func = &CPU::ADC;
    opcodeTable[0x75].length = 2;
    opcodeTable[0x75].cycles = 4;
    opcodeTable[0x75].mode = ZEROPAGE_X;
    
    // Opcode 0x76: ROR Zero Page,X
    opcodeTable[0x76].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x76].handler.func.uchar_ptr_func = &CPU::ROR;
    opcodeTable[0x76].length = 2;
    opcodeTable[0x76].cycles = 6;
    opcodeTable[0x76].mode = ZEROPAGE_X;
    
    // Opcode 0x77: Undefined
    opcodeTable[0x77].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x77].handler.func.uchar_ptr_func = &CPU::RRA;
    opcodeTable[0x77].length = 2;
    opcodeTable[0x77].cycles = 6;
    opcodeTable[0x77].mode = ZEROPAGE_X;

    // Opcode 0x78: SEI
    opcodeTable[0x78].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x78].handler.func.void_func = &CPU::SEI;
    opcodeTable[0x78].length = 1;
    opcodeTable[0x78].cycles = 2;
    opcodeTable[0x78].mode = IMPLIED;
    
    // Opcode 0x79: ADC Absolute,Y
    opcodeTable[0x79].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x79].handler.func.uchar_func = &CPU::ADC;
    opcodeTable[0x79].length = 3;
    opcodeTable[0x79].cycles = 4;
    opcodeTable[0x79].mode = ABSOLUTE_Y;
    
    // Opcode 0x7A: Undefined
    
    // Opcode 0x7B: Undefined
    opcodeTable[0x7B].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x7B].handler.func.uchar_ptr_func = &CPU::RRA;
    opcodeTable[0x7B].length = 3;
    opcodeTable[0x7B].cycles = 7;
    opcodeTable[0x7B].mode = ABSOLUTE_Y;

    // Opcode 0x7C: Undefined
    opcodeTable[0x7C].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x7C].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x7C].length = 3;
    opcodeTable[0x7C].cycles = 4;
    opcodeTable[0x7C].mode = ABSOLUTE_X;

    // Opcode 0x7D: ADC Absolute,X
    opcodeTable[0x7D].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x7D].handler.func.uchar_func = &CPU::ADC;
    opcodeTable[0x7D].length = 3;
    opcodeTable[0x7D].cycles = 4;
    opcodeTable[0x7D].mode = ABSOLUTE_X;
    
    // Opcode 0x7E: ROR Absolute,X
    opcodeTable[0x7E].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x7E].handler.func.uchar_ptr_func = &CPU::ROR;
    opcodeTable[0x7E].length = 3;
    opcodeTable[0x7E].cycles = 7;
    opcodeTable[0x7E].mode = ABSOLUTE_X;
    
    // Opcode 0x7F: Undefined
    opcodeTable[0x7F].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x7F].handler.func.uchar_ptr_func = &CPU::RRA;
    opcodeTable[0x7F].length = 3;
    opcodeTable[0x7F].cycles = 7;
    opcodeTable[0x7F].mode = ABSOLUTE_X;

    // Opcode 0x80: Undefined
    opcodeTable[0x80].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x80].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x80].length = 2;
    opcodeTable[0x80].cycles = 2;
    opcodeTable[0x80].mode = IMMEDIATE;

    // Opcode 0x81: STA (Indirect,X)
    opcodeTable[0x81].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x81].handler.func.ushort_func = &CPU::STA;
    opcodeTable[0x81].length = 2;
    opcodeTable[0x81].cycles = 6;
    opcodeTable[0x81].mode = INDIRECT_X;
    
    // Opcode 0x82: Undefined
    opcodeTable[0x82].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x82].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x82].length = 2;
    opcodeTable[0x82].cycles = 2;
    opcodeTable[0x82].mode = IMMEDIATE;

    // Opcode 0x83: Undefined
    opcodeTable[0x83].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x83].handler.func.uchar_ptr_func = &CPU::SAX;
    opcodeTable[0x83].length = 2;
    opcodeTable[0x83].cycles = 6;
    opcodeTable[0x83].mode = INDIRECT_X;
    
    // Opcode 0x84: STY Zero Page
    opcodeTable[0x84].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x84].handler.func.ushort_func = &CPU::STY;
    opcodeTable[0x84].length = 2;
    opcodeTable[0x84].cycles = 3;
    opcodeTable[0x84].mode = ZEROPAGE;
    
    // Opcode 0x85: STA Zero Page
    opcodeTable[0x85].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x85].handler.func.ushort_func = &CPU::STA;
    opcodeTable[0x85].length = 2;
    opcodeTable[0x85].cycles = 3;
    opcodeTable[0x85].mode = ZEROPAGE;
    
    // Opcode 0x86: STX Zero Page
    opcodeTable[0x86].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x86].handler.func.ushort_func = &CPU::STX;
    opcodeTable[0x86].length = 2;
    opcodeTable[0x86].cycles = 3;
    opcodeTable[0x86].mode = ZEROPAGE;
    
    // Opcode 0x87: Undefined
    opcodeTable[0x87].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x87].handler.func.uchar_ptr_func = &CPU::SAX;
    opcodeTable[0x87].length = 2;
    opcodeTable[0x87].cycles = 3;
    opcodeTable[0x87].mode = ZEROPAGE;

    // Opcode 0x88: DEY
    opcodeTable[0x88].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x88].handler.func.void_func = &CPU::DEY;
    opcodeTable[0x88].length = 1;
    opcodeTable[0x88].cycles = 2;
    opcodeTable[0x88].mode = IMPLIED;
    
    // Opcode 0x89: Undefined
    opcodeTable[0x89].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x89].handler.func.void_func = &CPU::NOP;
    opcodeTable[0x89].length = 2;
    opcodeTable[0x89].cycles = 2;
    opcodeTable[0x89].mode = IMMEDIATE;

    // Opcode 0x8A: TXA
    opcodeTable[0x8A].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x8A].handler.func.void_func = &CPU::TXA;
    opcodeTable[0x8A].length = 1;
    opcodeTable[0x8A].cycles = 2;
    opcodeTable[0x8A].mode = IMPLIED;
    
    // Opcode 0x8B: Undefined
    opcodeTable[0x8B].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0x8B].handler.func.uchar_func = &CPU::XAA;
    opcodeTable[0x8B].length = 2;
    opcodeTable[0x8B].cycles = 2;
    opcodeTable[0x8B].mode = IMMEDIATE;

    // Opcode 0x8C: STY Absolute
    opcodeTable[0x8C].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x8C].handler.func.ushort_func = &CPU::STY;
    opcodeTable[0x8C].length = 3;
    opcodeTable[0x8C].cycles = 4;
    opcodeTable[0x8C].mode = ABSOLUTE;
    
    // Opcode 0x8D: STA Absolute
    opcodeTable[0x8D].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x8D].handler.func.ushort_func = &CPU::STA;
    opcodeTable[0x8D].length = 3;
    opcodeTable[0x8D].cycles = 4;
    opcodeTable[0x8D].mode = ABSOLUTE;
    
    // Opcode 0x8E: STX Absolute
    opcodeTable[0x8E].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x8E].handler.func.ushort_func = &CPU::STX;
    opcodeTable[0x8E].length = 3;
    opcodeTable[0x8E].cycles = 4;
    opcodeTable[0x8E].mode = ABSOLUTE;
    
    // Opcode 0x8F: Undefined
    opcodeTable[0x8F].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x8F].handler.func.uchar_ptr_func = &CPU::SAX;
    opcodeTable[0x8F].length = 3;
    opcodeTable[0x8F].cycles = 4;
    opcodeTable[0x8F].mode = ABSOLUTE;

    // Opcode 0x90: BCC
    opcodeTable[0x90].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x90].handler.func.void_func = &CPU::BCC;
    opcodeTable[0x90].length = 2;
    opcodeTable[0x90].cycles = 2;
    opcodeTable[0x90].mode = RELATIVE;
    
    // Opcode 0x91: STA (Indirect),Y
    opcodeTable[0x91].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x91].handler.func.ushort_func = &CPU::STA;
    opcodeTable[0x91].length = 2;
    opcodeTable[0x91].cycles = 6;
    opcodeTable[0x91].mode = INDIRECT_Y;
    
    // Opcode 0x92: Undefined
    opcodeTable[0x92].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x92].handler.func.void_func = &CPU::STP;
    opcodeTable[0x92].length = 1;
    opcodeTable[0x92].cycles = 3;
    opcodeTable[0x92].mode = IMPLIED;

    // Opcode 0x93: Undefined
    
    // Opcode 0x94: STY Zero Page,X
    opcodeTable[0x94].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x94].handler.func.ushort_func = &CPU::STY;
    opcodeTable[0x94].length = 2;
    opcodeTable[0x94].cycles = 4;
    opcodeTable[0x94].mode = ZEROPAGE_X;
    
    // Opcode 0x95: STA Zero Page,X
    opcodeTable[0x95].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x95].handler.func.ushort_func = &CPU::STA;
    opcodeTable[0x95].length = 2;
    opcodeTable[0x95].cycles = 4;
    opcodeTable[0x95].mode = ZEROPAGE_X;
    
    // Opcode 0x96: STX Zero Page,Y
    opcodeTable[0x96].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x96].handler.func.ushort_func = &CPU::STX;
    opcodeTable[0x96].length = 2;
    opcodeTable[0x96].cycles = 4;
    opcodeTable[0x96].mode = ZEROPAGE_Y;
    
    // Opcode 0x97: Undefined
    opcodeTable[0x97].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0x97].handler.func.uchar_ptr_func = &CPU::SAX;
    opcodeTable[0x97].length = 2;
    opcodeTable[0x97].cycles = 4;
    opcodeTable[0x97].mode = ZEROPAGE_Y;

    // Opcode 0x98: TYA
    opcodeTable[0x98].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x98].handler.func.void_func = &CPU::TYA;
    opcodeTable[0x98].length = 1;
    opcodeTable[0x98].cycles = 2;
    opcodeTable[0x98].mode = IMPLIED;
    
    // Opcode 0x99: STA Absolute,Y
    opcodeTable[0x99].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x99].handler.func.ushort_func = &CPU::STA;
    opcodeTable[0x99].length = 3;
    opcodeTable[0x99].cycles = 5;
    opcodeTable[0x99].mode = ABSOLUTE_Y;
    
    // Opcode 0x9A: TXS
    opcodeTable[0x9A].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0x9A].handler.func.void_func = &CPU::TXS;
    opcodeTable[0x9A].length = 1;
    opcodeTable[0x9A].cycles = 2;
    opcodeTable[0x9A].mode = IMPLIED;
    
    // Opcode 0x9B: Undefined
    
    // Opcode 0x9C: Undefined
    opcodeTable[0x9C].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x9C].handler.func.ushort_func = &CPU::SHY;
    opcodeTable[0x9C].length = 3;
    opcodeTable[0x9C].cycles = 5;
    opcodeTable[0x9C].mode = ABSOLUTE_X;
    
    // Opcode 0x9D: STA Absolute,X
    opcodeTable[0x9D].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x9D].handler.func.ushort_func = &CPU::STA;
    opcodeTable[0x9D].length = 3;
    opcodeTable[0x9D].cycles = 5;
    opcodeTable[0x9D].mode = ABSOLUTE_X;
    
    // Opcode 0x9E: Undefined
    opcodeTable[0x9E].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0x9E].handler.func.ushort_func = &CPU::SHX;
    opcodeTable[0x9E].length = 3;
    opcodeTable[0x9E].cycles = 5;
    opcodeTable[0x9E].mode = ABSOLUTE_Y;

    // Opcode 0x9F: Undefined
    
    // Opcode 0xA0: LDY Immediate
    opcodeTable[0xA0].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xA0].handler.func.ushort_func = &CPU::LDY;
    opcodeTable[0xA0].length = 2;
    opcodeTable[0xA0].cycles = 2;
    opcodeTable[0xA0].mode = IMMEDIATE;
    
    // Opcode 0xA1: LDA (Indirect,X)
    opcodeTable[0xA1].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xA1].handler.func.ushort_func = &CPU::LDA;
    opcodeTable[0xA1].length = 2;
    opcodeTable[0xA1].cycles = 6;
    opcodeTable[0xA1].mode = INDIRECT_X;
    
    // Opcode 0xA2: LDX Immediate
    opcodeTable[0xA2].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xA2].handler.func.ushort_func = &CPU::LDX;
    opcodeTable[0xA2].length = 2;
    opcodeTable[0xA2].cycles = 2;
    opcodeTable[0xA2].mode = IMMEDIATE;
    
    // Opcode 0xA3: Undefined
    opcodeTable[0xA3].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xA3].handler.func.ushort_func = &CPU::LAX;
    opcodeTable[0xA3].length = 2;
    opcodeTable[0xA3].cycles = 6;
    opcodeTable[0xA3].mode = INDIRECT_X;

    // Opcode 0xA4: LDY Zero Page
    opcodeTable[0xA4].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xA4].handler.func.ushort_func = &CPU::LDY;
    opcodeTable[0xA4].length = 2;
    opcodeTable[0xA4].cycles = 3;
    opcodeTable[0xA4].mode = ZEROPAGE;
    
    // Opcode 0xA5: LDA Zero Page
    opcodeTable[0xA5].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xA5].handler.func.ushort_func = &CPU::LDA;
    opcodeTable[0xA5].length = 2;
    opcodeTable[0xA5].cycles = 3;
    opcodeTable[0xA5].mode = ZEROPAGE;
    
    // Opcode 0xA6: LDX Zero Page
    opcodeTable[0xA6].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xA6].handler.func.ushort_func = &CPU::LDX;
    opcodeTable[0xA6].length = 2;
    opcodeTable[0xA6].cycles = 3;
    opcodeTable[0xA6].mode = ZEROPAGE;
    
    // Opcode 0xA7: Undefined
    opcodeTable[0xA7].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xA7].handler.func.ushort_func = &CPU::LAX;
    opcodeTable[0xA7].length = 2;
    opcodeTable[0xA7].cycles = 3;
    opcodeTable[0xA7].mode = ZEROPAGE;

    // Opcode 0xA8: TAY
    opcodeTable[0xA8].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xA8].handler.func.void_func = &CPU::TAY;
    opcodeTable[0xA8].length = 1;
    opcodeTable[0xA8].cycles = 2;
    opcodeTable[0xA8].mode = IMPLIED;
    
    // Opcode 0xA9: LDA Immediate
    opcodeTable[0xA9].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xA9].handler.func.ushort_func = &CPU::LDA;
    opcodeTable[0xA9].length = 2;
    opcodeTable[0xA9].cycles = 2;
    opcodeTable[0xA9].mode = IMMEDIATE;
    
    // Opcode 0xAA: TAX
    opcodeTable[0xAA].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xAA].handler.func.void_func = &CPU::TAX;
    opcodeTable[0xAA].length = 1;
    opcodeTable[0xAA].cycles = 2;
    opcodeTable[0xAA].mode = IMPLIED;
    
    // Opcode 0xAB: Undefined
    opcodeTable[0xAB].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xAB].handler.func.ushort_func = &CPU::LAX;
    opcodeTable[0xAB].length = 2;
    opcodeTable[0xAB].cycles = 2;
    opcodeTable[0xAB].mode = IMMEDIATE;

    // Opcode 0xAC: LDY Absolute
    opcodeTable[0xAC].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xAC].handler.func.ushort_func = &CPU::LDY;
    opcodeTable[0xAC].length = 3;
    opcodeTable[0xAC].cycles = 4;
    opcodeTable[0xAC].mode = ABSOLUTE;
    
    // Opcode 0xAD: LDA Absolute
    opcodeTable[0xAD].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xAD].handler.func.ushort_func = &CPU::LDA;
    opcodeTable[0xAD].length = 3;
    opcodeTable[0xAD].cycles = 4;
    opcodeTable[0xAD].mode = ABSOLUTE;
    
    // Opcode 0xAE: LDX Absolute
    opcodeTable[0xAE].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xAE].handler.func.ushort_func = &CPU::LDX;
    opcodeTable[0xAE].length = 3;
    opcodeTable[0xAE].cycles = 4;
    opcodeTable[0xAE].mode = ABSOLUTE;
    
    // Opcode 0xAF: Undefined
    opcodeTable[0xAF].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xAF].handler.func.ushort_func = &CPU::LAX;
    opcodeTable[0xAF].length = 3;
    opcodeTable[0xAF].cycles = 4;
    opcodeTable[0xAF].mode = ABSOLUTE;

    // Opcode 0xB0: BCS
    opcodeTable[0xB0].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xB0].handler.func.void_func = &CPU::BCS;
    opcodeTable[0xB0].length = 2;
    opcodeTable[0xB0].cycles = 2;
    opcodeTable[0xB0].mode = RELATIVE;
    
    // Opcode 0xB1: LDA (Indirect),Y
    opcodeTable[0xB1].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xB1].handler.func.ushort_func = &CPU::LDA;
    opcodeTable[0xB1].length = 2;
    opcodeTable[0xB1].cycles = 5;
    opcodeTable[0xB1].mode = INDIRECT_Y;
    
    // Opcode 0xB2: Undefined
    opcodeTable[0xB2].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xB2].handler.func.void_func = &CPU::STP;
    opcodeTable[0xB2].length = 1;
    opcodeTable[0xB2].cycles = 3;
    opcodeTable[0xB2].mode = IMPLIED;

    // Opcode 0xB3: Undefined
    opcodeTable[0xB3].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xB3].handler.func.ushort_func = &CPU::LAX;
    opcodeTable[0xB3].length = 2;
    opcodeTable[0xB3].cycles = 5;
    opcodeTable[0xB3].mode = INDIRECT_Y;

    // Opcode 0xB4: LDY Zero Page,X
    opcodeTable[0xB4].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xB4].handler.func.ushort_func = &CPU::LDY;
    opcodeTable[0xB4].length = 2;
    opcodeTable[0xB4].cycles = 4;
    opcodeTable[0xB4].mode = ZEROPAGE_X;
    
    // Opcode 0xB5: LDA Zero Page,X
    opcodeTable[0xB5].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xB5].handler.func.ushort_func = &CPU::LDA;
    opcodeTable[0xB5].length = 2;
    opcodeTable[0xB5].cycles = 4;
    opcodeTable[0xB5].mode = ZEROPAGE_X;
    
    // Opcode 0xB6: LDX Zero Page,Y
    opcodeTable[0xB6].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xB6].handler.func.ushort_func = &CPU::LDX;
    opcodeTable[0xB6].length = 2;
    opcodeTable[0xB6].cycles = 4;
    opcodeTable[0xB6].mode = ZEROPAGE_Y;
    
    // Opcode 0xB7: Undefined
    opcodeTable[0xB7].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xB7].handler.func.ushort_func = &CPU::LAX;
    opcodeTable[0xB7].length = 2;
    opcodeTable[0xB7].cycles = 4;
    opcodeTable[0xB7].mode = ZEROPAGE_Y;

    // Opcode 0xB8: CLV
    opcodeTable[0xB8].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xB8].handler.func.void_func = &CPU::CLV;
    opcodeTable[0xB8].length = 1;
    opcodeTable[0xB8].cycles = 2;
    opcodeTable[0xB8].mode = IMPLIED;
    
    // Opcode 0xB9: LDA Absolute,Y
    opcodeTable[0xB9].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xB9].handler.func.ushort_func = &CPU::LDA;
    opcodeTable[0xB9].length = 3;
    opcodeTable[0xB9].cycles = 4;
    opcodeTable[0xB9].mode = ABSOLUTE_Y;
    
    // Opcode 0xBA: TSX
    opcodeTable[0xBA].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xBA].handler.func.void_func = &CPU::TSX;
    opcodeTable[0xBA].length = 1;
    opcodeTable[0xBA].cycles = 2;
    opcodeTable[0xBA].mode = IMPLIED;
    
    // Opcode 0xBB: Undefined
    opcodeTable[0xBB].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xBB].handler.func.uchar_func = &CPU::LAS;
    opcodeTable[0xBB].length = 3;
    opcodeTable[0xBB].cycles = 4;
    opcodeTable[0xBB].mode = ABSOLUTE_Y;

    // Opcode 0xBC: LDY Absolute,X
    opcodeTable[0xBC].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xBC].handler.func.ushort_func = &CPU::LDY;
    opcodeTable[0xBC].length = 3;
    opcodeTable[0xBC].cycles = 4;
    opcodeTable[0xBC].mode = ABSOLUTE_X;
    
    // Opcode 0xBD: LDA Absolute,X
    opcodeTable[0xBD].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xBD].handler.func.ushort_func = &CPU::LDA;
    opcodeTable[0xBD].length = 3;
    opcodeTable[0xBD].cycles = 4;
    opcodeTable[0xBD].mode = ABSOLUTE_X;
    
    // Opcode 0xBE: LDX Absolute,Y
    opcodeTable[0xBE].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xBE].handler.func.ushort_func = &CPU::LDX;
    opcodeTable[0xBE].length = 3;
    opcodeTable[0xBE].cycles = 4;
    opcodeTable[0xBE].mode = ABSOLUTE_Y;
    
    // Opcode 0xBF: Undefined
    opcodeTable[0xBF].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xBF].handler.func.ushort_func = &CPU::LAX;
    opcodeTable[0xBF].length = 3;
    opcodeTable[0xBF].cycles = 4;
    opcodeTable[0xBF].mode = ABSOLUTE_Y;

    // Opcode 0xC0: CPY Immediate
    opcodeTable[0xC0].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xC0].handler.func.uchar_func = &CPU::CPY;
    opcodeTable[0xC0].length = 2;
    opcodeTable[0xC0].cycles = 2;
    opcodeTable[0xC0].mode = IMMEDIATE;
    
    // Opcode 0xC1: CMP (Indirect,X)
    opcodeTable[0xC1].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xC1].handler.func.uchar_func = &CPU::CMP;
    opcodeTable[0xC1].length = 2;
    opcodeTable[0xC1].cycles = 6;
    opcodeTable[0xC1].mode = INDIRECT_X;
    
    // Opcode 0xC2: Undefined
    opcodeTable[0xC2].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xC2].handler.func.void_func = &CPU::NOP;
    opcodeTable[0xC2].length = 2;
    opcodeTable[0xC2].cycles = 2;
    opcodeTable[0xC2].mode = IMMEDIATE;

    // Opcode 0xC3: Undefined
    opcodeTable[0xC3].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xC3].handler.func.uchar_ptr_func = &CPU::DCP;
    opcodeTable[0xC3].length = 2;
    opcodeTable[0xC3].cycles = 8;
    opcodeTable[0xC3].mode = INDIRECT_X;    

    // Opcode 0xC4: CPY Zero Page
    opcodeTable[0xC4].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xC4].handler.func.uchar_func = &CPU::CPY;
    opcodeTable[0xC4].length = 2;
    opcodeTable[0xC4].cycles = 3;
    opcodeTable[0xC4].mode = ZEROPAGE;
    
    // Opcode 0xC5: CMP Zero Page
    opcodeTable[0xC5].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xC5].handler.func.uchar_func = &CPU::CMP;
    opcodeTable[0xC5].length = 2;
    opcodeTable[0xC5].cycles = 3;
    opcodeTable[0xC5].mode = ZEROPAGE;
    
    // Opcode 0xC6: DEC Zero Page
    opcodeTable[0xC6].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xC6].handler.func.ushort_func = &CPU::DEC;
    opcodeTable[0xC6].length = 2;
    opcodeTable[0xC6].cycles = 5;
    opcodeTable[0xC6].mode = ZEROPAGE;
    
    // Opcode 0xC7: Undefined
    opcodeTable[0xC7].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xC7].handler.func.uchar_ptr_func = &CPU::DCP;
    opcodeTable[0xC7].length = 2;
    opcodeTable[0xC7].cycles = 5;
    opcodeTable[0xC7].mode = ZEROPAGE;

    // Opcode 0xC8: INY
    opcodeTable[0xC8].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xC8].handler.func.void_func = &CPU::INY;
    opcodeTable[0xC8].length = 1;
    opcodeTable[0xC8].cycles = 2;
    opcodeTable[0xC8].mode = IMPLIED;
    
    // Opcode 0xC9: CMP Immediate
    opcodeTable[0xC9].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xC9].handler.func.uchar_func = &CPU::CMP;
    opcodeTable[0xC9].length = 2;
    opcodeTable[0xC9].cycles = 2;
    opcodeTable[0xC9].mode = IMMEDIATE;
    
    // Opcode 0xCA: DEX
    opcodeTable[0xCA].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xCA].handler.func.void_func = &CPU::DEX;
    opcodeTable[0xCA].length = 1;
    opcodeTable[0xCA].cycles = 2;
    opcodeTable[0xCA].mode = IMPLIED;
    
    // Opcode 0xCB: Undefined
    opcodeTable[0xCB].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xCB].handler.func.uchar_func = &CPU::SBX;
    opcodeTable[0xCB].length = 2;
    opcodeTable[0xCB].cycles = 2;
    opcodeTable[0xCB].mode = IMMEDIATE;

    // Opcode 0xCC: CPY Absolute
    opcodeTable[0xCC].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xCC].handler.func.uchar_func = &CPU::CPY;
    opcodeTable[0xCC].length = 3;
    opcodeTable[0xCC].cycles = 4;
    opcodeTable[0xCC].mode = ABSOLUTE;
    
    // Opcode 0xCD: CMP Absolute
    opcodeTable[0xCD].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xCD].handler.func.uchar_func = &CPU::CMP;
    opcodeTable[0xCD].length = 3;
    opcodeTable[0xCD].cycles = 4;
    opcodeTable[0xCD].mode = ABSOLUTE;
    
    // Opcode 0xCE: DEC Absolute
    opcodeTable[0xCE].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xCE].handler.func.ushort_func = &CPU::DEC;
    opcodeTable[0xCE].length = 3;
    opcodeTable[0xCE].cycles = 6;
    opcodeTable[0xCE].mode = ABSOLUTE;
    
    // Opcode 0xCF: Undefined
    opcodeTable[0xCF].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xCF].handler.func.uchar_ptr_func = &CPU::DCP;
    opcodeTable[0xCF].length = 3;
    opcodeTable[0xCF].cycles = 6;
    opcodeTable[0xCF].mode = ABSOLUTE;

    // Opcode 0xD0: BNE
    opcodeTable[0xD0].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xD0].handler.func.void_func = &CPU::BNE;
    opcodeTable[0xD0].length = 2;
    opcodeTable[0xD0].cycles = 2;
    opcodeTable[0xD0].mode = RELATIVE;
    
    // Opcode 0xD1: CMP (Indirect),Y
    opcodeTable[0xD1].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xD1].handler.func.uchar_func = &CPU::CMP;
    opcodeTable[0xD1].length = 2;
    opcodeTable[0xD1].cycles = 5;
    opcodeTable[0xD1].mode = INDIRECT_Y;
    
    // Opcode 0xD2: Undefined
    opcodeTable[0xD2].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xD2].handler.func.void_func = &CPU::STP;
    opcodeTable[0xD2].length = 1;
    opcodeTable[0xD2].cycles = 3;
    opcodeTable[0xD2].mode = IMPLIED;

    // Opcode 0xD3: Undefined
    opcodeTable[0xD3].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xD3].handler.func.uchar_ptr_func = &CPU::DCP;
    opcodeTable[0xD3].length = 2;
    opcodeTable[0xD3].cycles = 8;
    opcodeTable[0xD3].mode = INDIRECT_Y;

    // Opcode 0xD4: Undefined
    opcodeTable[0xD4].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xD4].handler.func.void_func = &CPU::NOP;
    opcodeTable[0xD4].length = 2;
    opcodeTable[0xD4].cycles = 4;
    opcodeTable[0xD4].mode = ZEROPAGE_X;

    // Opcode 0xD5: CMP Zero Page,X
    opcodeTable[0xD5].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xD5].handler.func.uchar_func = &CPU::CMP;
    opcodeTable[0xD5].length = 2;
    opcodeTable[0xD5].cycles = 4;
    opcodeTable[0xD5].mode = ZEROPAGE_X;
    
    // Opcode 0xD6: DEC Zero Page,X
    opcodeTable[0xD6].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xD6].handler.func.ushort_func = &CPU::DEC;
    opcodeTable[0xD6].length = 2;
    opcodeTable[0xD6].cycles = 6;
    opcodeTable[0xD6].mode = ZEROPAGE_X;
    
    // Opcode 0xD7: Undefined
    opcodeTable[0xD7].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xD7].handler.func.uchar_ptr_func = &CPU::DCP;
    opcodeTable[0xD7].length = 2;
    opcodeTable[0xD7].cycles = 6;
    opcodeTable[0xD7].mode = ZEROPAGE_X;

    // Opcode 0xD8: CLD
    opcodeTable[0xD8].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xD8].handler.func.void_func = &CPU::CLD;
    opcodeTable[0xD8].length = 1;
    opcodeTable[0xD8].cycles = 2;
    opcodeTable[0xD8].mode = IMPLIED;
    
    // Opcode 0xD9: CMP Absolute,Y
    opcodeTable[0xD9].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xD9].handler.func.uchar_func = &CPU::CMP;
    opcodeTable[0xD9].length = 3;
    opcodeTable[0xD9].cycles = 4;
    opcodeTable[0xD9].mode = ABSOLUTE_Y;
    
    // Opcode 0xDA: Undefined
    
    // Opcode 0xDB: Undefined
    opcodeTable[0xDB].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xDB].handler.func.uchar_ptr_func = &CPU::DCP;
    opcodeTable[0xDB].length = 3;
    opcodeTable[0xDB].cycles = 7;
    opcodeTable[0xDB].mode = ABSOLUTE_Y;

    // Opcode 0xDC: Undefined
    opcodeTable[0xDC].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xDC].handler.func.void_func = &CPU::NOP;
    opcodeTable[0xDC].length = 3;
    opcodeTable[0xDC].cycles = 4;
    opcodeTable[0xDC].mode = ABSOLUTE_X;

    // Opcode 0xDD: CMP Absolute,X
    opcodeTable[0xDD].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xDD].handler.func.uchar_func = &CPU::CMP;
    opcodeTable[0xDD].length = 3;
    opcodeTable[0xDD].cycles = 4;
    opcodeTable[0xDD].mode = ABSOLUTE_X;
    
    // Opcode 0xDE: DEC Absolute,X
    opcodeTable[0xDE].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xDE].handler.func.ushort_func = &CPU::DEC;
    opcodeTable[0xDE].length = 3;
    opcodeTable[0xDE].cycles = 7;
    opcodeTable[0xDE].mode = ABSOLUTE_X;
    
    // Opcode 0xDF: Undefined
    opcodeTable[0xDF].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xDF].handler.func.uchar_ptr_func = &CPU::DCP;
    opcodeTable[0xDF].length = 3;
    opcodeTable[0xDF].cycles = 7;
    opcodeTable[0xDF].mode = ABSOLUTE_X;

    // Opcode 0xE0: CPX Immediate
    opcodeTable[0xE0].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xE0].handler.func.uchar_func = &CPU::CPX;
    opcodeTable[0xE0].length = 2;
    opcodeTable[0xE0].cycles = 2;
    opcodeTable[0xE0].mode = IMMEDIATE;
    
    // Opcode 0xE1: SBC (Indirect,X)
    opcodeTable[0xE1].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xE1].handler.func.uchar_func = &CPU::SBC;
    opcodeTable[0xE1].length = 2;
    opcodeTable[0xE1].cycles = 6;
    opcodeTable[0xE1].mode = INDIRECT_X;
    
    // Opcode 0xE2: Undefined
    opcodeTable[0xE2].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xE2].handler.func.void_func = &CPU::NOP;
    opcodeTable[0xE2].length = 2;
    opcodeTable[0xE2].cycles = 2;
    opcodeTable[0xE2].mode = IMMEDIATE;

    // Opcode 0xE3: Undefined
    opcodeTable[0xE3].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xE3].handler.func.uchar_ptr_func = &CPU::ISC;
    opcodeTable[0xE3].length = 2;
    opcodeTable[0xE3].cycles = 8;
    opcodeTable[0xE3].mode = INDIRECT_X;

    // Opcode 0xE4: CPX Zero Page
    opcodeTable[0xE4].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xE4].handler.func.uchar_func = &CPU::CPX;
    opcodeTable[0xE4].length = 2;
    opcodeTable[0xE4].cycles = 3;
    opcodeTable[0xE4].mode = ZEROPAGE;
    
    // Opcode 0xE5: SBC Zero Page
    opcodeTable[0xE5].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xE5].handler.func.uchar_func = &CPU::SBC;
    opcodeTable[0xE5].length = 2;
    opcodeTable[0xE5].cycles = 3;
    opcodeTable[0xE5].mode = ZEROPAGE;
    
    // Opcode 0xE6: INC Zero Page
    opcodeTable[0xE6].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xE6].handler.func.ushort_func = &CPU::INC;
    opcodeTable[0xE6].length = 2;
    opcodeTable[0xE6].cycles = 5;
    opcodeTable[0xE6].mode = ZEROPAGE;
    
    // Opcode 0xE7: Undefined
    opcodeTable[0xE7].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xE7].handler.func.uchar_ptr_func = &CPU::ISC;
    opcodeTable[0xE7].length = 2;
    opcodeTable[0xE7].cycles = 5;
    opcodeTable[0xE7].mode = ZEROPAGE;

    // Opcode 0xE8: INX
    opcodeTable[0xE8].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xE8].handler.func.void_func = &CPU::INX;
    opcodeTable[0xE8].length = 1;
    opcodeTable[0xE8].cycles = 2;
    opcodeTable[0xE8].mode = IMPLIED;
    
    // Opcode 0xE9: SBC Immediate
    opcodeTable[0xE9].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xE9].handler.func.uchar_func = &CPU::SBC;
    opcodeTable[0xE9].length = 2;
    opcodeTable[0xE9].cycles = 2;
    opcodeTable[0xE9].mode = IMMEDIATE;
    
    // Opcode 0xEA: NOP
    opcodeTable[0xEA].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xEA].handler.func.void_func = &CPU::NOP;
    opcodeTable[0xEA].length = 1;
    opcodeTable[0xEA].cycles = 2;
    opcodeTable[0xEA].mode = IMPLIED;
    
    // Opcode 0xEB: Undefined
    opcodeTable[0xEB].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xEB].handler.func.uchar_func = &CPU::SBC;
    opcodeTable[0xEB].length = 2;
    opcodeTable[0xEB].cycles = 2;
    opcodeTable[0xEB].mode = IMMEDIATE;

    // Opcode 0xEC: CPX Absolute
    opcodeTable[0xEC].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xEC].handler.func.uchar_func = &CPU::CPX;
    opcodeTable[0xEC].length = 3;
    opcodeTable[0xEC].cycles = 4;
    opcodeTable[0xEC].mode = ABSOLUTE;
    
    // Opcode 0xED: SBC Absolute
    opcodeTable[0xED].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xED].handler.func.uchar_func = &CPU::SBC;
    opcodeTable[0xED].length = 3;
    opcodeTable[0xED].cycles = 4;
    opcodeTable[0xED].mode = ABSOLUTE;
    
    // Opcode 0xEE: INC Absolute
    opcodeTable[0xEE].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xEE].handler.func.ushort_func = &CPU::INC;
    opcodeTable[0xEE].length = 3;
    opcodeTable[0xEE].cycles = 6;
    opcodeTable[0xEE].mode = ABSOLUTE;
    
    // Opcode 0xEF: Undefined
    opcodeTable[0xEF].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xEF].handler.func.uchar_ptr_func = &CPU::ISC;
    opcodeTable[0xEF].length = 3;
    opcodeTable[0xEF].cycles = 6;
    opcodeTable[0xEF].mode = ABSOLUTE;

    // Opcode 0xF0: BEQ
    opcodeTable[0xF0].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xF0].handler.func.void_func = &CPU::BEQ;
    opcodeTable[0xF0].length = 2;
    opcodeTable[0xF0].cycles = 2;
    opcodeTable[0xF0].mode = RELATIVE;
    
    // Opcode 0xF1: SBC (Indirect),Y
    opcodeTable[0xF1].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xF1].handler.func.uchar_func = &CPU::SBC;
    opcodeTable[0xF1].length = 2;
    opcodeTable[0xF1].cycles = 5;
    opcodeTable[0xF1].mode = INDIRECT_Y;
    
    // Opcode 0xF2: Undefined
    opcodeTable[0xF2].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xF2].handler.func.void_func = &CPU::STP;
    opcodeTable[0xF2].length = 1;
    opcodeTable[0xF2].cycles = 3;
    opcodeTable[0xF2].mode = IMPLIED;

    // Opcode 0xF3: Undefined
    opcodeTable[0xF3].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xF3].handler.func.uchar_ptr_func = &CPU::ISC;
    opcodeTable[0xF3].length = 2;
    opcodeTable[0xF3].cycles = 8;
    opcodeTable[0xF3].mode = INDIRECT_Y;

    // Opcode 0xF4: Undefined
    opcodeTable[0xF4].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xF4].handler.func.void_func = &CPU::NOP;
    opcodeTable[0xF4].length = 2;
    opcodeTable[0xF4].cycles = 4;
    opcodeTable[0xF4].mode = ZEROPAGE_X;

    // Opcode 0xF5: SBC Zero Page,X
    opcodeTable[0xF5].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xF5].handler.func.uchar_func = &CPU::SBC;
    opcodeTable[0xF5].length = 2;
    opcodeTable[0xF5].cycles = 4;
    opcodeTable[0xF5].mode = ZEROPAGE_X;
    
    // Opcode 0xF6: INC Zero Page,X
    opcodeTable[0xF6].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xF6].handler.func.ushort_func = &CPU::INC;
    opcodeTable[0xF6].length = 2;
    opcodeTable[0xF6].cycles = 6;
    opcodeTable[0xF6].mode = ZEROPAGE_X;
    
    // Opcode 0xF7: Undefined
    opcodeTable[0xF7].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xF7].handler.func.uchar_ptr_func = &CPU::ISC;
    opcodeTable[0xF7].length = 2;
    opcodeTable[0xF7].cycles = 6;
    opcodeTable[0xF7].mode = ZEROPAGE_X;

    // Opcode 0xF8: SED
    opcodeTable[0xF8].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xF8].handler.func.void_func = &CPU::SED;
    opcodeTable[0xF8].length = 1;
    opcodeTable[0xF8].cycles = 2;
    opcodeTable[0xF8].mode = IMPLIED;
    
    // Opcode 0xF9: SBC Absolute,Y
    opcodeTable[0xF9].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xF9].handler.func.uchar_func = &CPU::SBC;
    opcodeTable[0xF9].length = 3;
    opcodeTable[0xF9].cycles = 4;
    opcodeTable[0xF9].mode = ABSOLUTE_Y;
    
    // Opcode 0xFA: Undefined
    
    // Opcode 0xFB: Undefined
    opcodeTable[0xFB].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xFB].handler.func.uchar_ptr_func = &CPU::ISC;
    opcodeTable[0xFB].length = 3;
    opcodeTable[0xFB].cycles = 7;
    opcodeTable[0xFB].mode = ABSOLUTE_Y;

    // Opcode 0xFC: Undefined
    opcodeTable[0xFC].handler.type = InstructionHandler::TYPE_VOID;
    opcodeTable[0xFC].handler.func.void_func = &CPU::NOP;
    opcodeTable[0xFC].length = 3;
    opcodeTable[0xFC].cycles = 4;
    opcodeTable[0xFC].mode = ABSOLUTE_X;

    // Opcode 0xFD: SBC Absolute,X
    opcodeTable[0xFD].handler.type = InstructionHandler::TYPE_UCHAR;
    opcodeTable[0xFD].handler.func.uchar_func = &CPU::SBC;
    opcodeTable[0xFD].length = 3;
    opcodeTable[0xFD].cycles = 4;
    opcodeTable[0xFD].mode = ABSOLUTE_X;
    
    // Opcode 0xFE: INC Absolute,X
    opcodeTable[0xFE].handler.type = InstructionHandler::TYPE_USHORT;
    opcodeTable[0xFE].handler.func.ushort_func = &CPU::INC;
    opcodeTable[0xFE].length = 3;
    opcodeTable[0xFE].cycles = 7;
    opcodeTable[0xFE].mode = ABSOLUTE_X;
    
    // Opcode 0xFF: Undefined
    opcodeTable[0xFF].handler.type = InstructionHandler::TYPE_UCHAR_PTR;
    opcodeTable[0xFF].handler.func.uchar_ptr_func = &CPU::ISC;
    opcodeTable[0xFF].length = 3;
    opcodeTable[0xFF].cycles = 7;
    opcodeTable[0xFF].mode = ABSOLUTE_X;
}