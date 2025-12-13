#include "include/CPU.h"

void CPU :: initializeOpcodeTable() {
    // Initialize all opcodes as NOP first
    for (int i = 0; i < 256; i++) {
        opcodeTable[i] = {
            .handler = {
                .type = InstructionHandler::TYPE_VOID,
                .func = { .void_func = NOP }  // Default to NOP for undefined opcodes
            },
            .length = 1,
            .cycles = 2,
            .mode = IMPLIED
        };
    }
    
    // Opcode 0x00: BRK
    opcodeTable[0x00] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = BRK }
        },
        .length = 2,
        .cycles = 7,
        .mode = IMPLIED
    };
    
    // Opcode 0x01: ORA (Indirect,X)
    opcodeTable[0x01] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ORA }
        },
        .length = 2,
        .cycles = 6,
        .mode = INDIRECT_X
    };
    
    // Opcode 0x02: Undefined
    // Already set to NOP
    
    // Opcode 0x03: Undefined
    
    // Opcode 0x04: Undefined
    
    // Opcode 0x05: ORA Zero Page
    opcodeTable[0x05] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ORA }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x06: ASL Zero Page
    opcodeTable[0x06] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ASL }
        },
        .length = 2,
        .cycles = 5,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x07: Undefined
    
    // Opcode 0x08: PHP
    opcodeTable[0x08] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = PHP }
        },
        .length = 1,
        .cycles = 3,
        .mode = IMPLIED
    };
    
    // Opcode 0x09: ORA Immediate
    opcodeTable[0x09] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ORA }
        },
        .length = 2,
        .cycles = 2,
        .mode = IMMEDIATE
    };
    
    // Opcode 0x0A: ASL Accumulator
    opcodeTable[0x0A] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ASL }
        },
        .length = 1,
        .cycles = 2,
        .mode = ACCUMULATOR
    };
    
    // Opcode 0x0B: Undefined
    
    // Opcode 0x0C: Undefined
    
    // Opcode 0x0D: ORA Absolute
    opcodeTable[0x0D] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ORA }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x0E: ASL Absolute
    opcodeTable[0x0E] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ASL }
        },
        .length = 3,
        .cycles = 6,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x0F: Undefined
    
    // Opcode 0x10: BPL
    opcodeTable[0x10] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = BPL }
        },
        .length = 2,
        .cycles = 2,
        .mode = RELATIVE
    };
    
    // Opcode 0x11: ORA (Indirect),Y
    opcodeTable[0x11] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ORA }
        },
        .length = 2,
        .cycles = 5,
        .mode = INDIRECT_Y
    };
    
    // Opcode 0x12: Undefined
    
    // Opcode 0x13: Undefined
    
    // Opcode 0x14: Undefined
    
    // Opcode 0x15: ORA Zero Page,X
    opcodeTable[0x15] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ORA }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0x16: ASL Zero Page,X
    opcodeTable[0x16] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ASL }
        },
        .length = 2,
        .cycles = 6,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0x17: Undefined
    
    // Opcode 0x18: CLC
    opcodeTable[0x18] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = CLC }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0x19: ORA Absolute,Y
    opcodeTable[0x19] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .uchar_func = ORA }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_Y
    };
    
    // Opcode 0x1A: Undefined
    
    // Opcode 0x1B: Undefined
    
    // Opcode 0x1C: Undefined
    
    // Opcode 0x1D: ORA Absolute,X
    opcodeTable[0x1D] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .uchar_func = ORA }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0x1E: ASL Absolute,X
    opcodeTable[0x1E] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ASL }
        },
        .length = 3,
        .cycles = 7,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0x1F: Undefined
    
    // Opcode 0x20: JSR
    opcodeTable[0x20] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = JSR }
        },
        .length = 3,
        .cycles = 6,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x21: AND (Indirect,X)
    opcodeTable[0x21] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = AND }
        },
        .length = 2,
        .cycles = 6,
        .mode = INDIRECT_X
    };
    
    // Opcode 0x22: Undefined
    
    // Opcode 0x23: Undefined
    
    // Opcode 0x24: BIT Zero Page
    opcodeTable[0x24] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = BIT }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x25: AND Zero Page
    opcodeTable[0x25] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = AND }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x26: ROL Zero Page
    opcodeTable[0x26] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ROL }
        },
        .length = 2,
        .cycles = 5,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x27: Undefined
    
    // Opcode 0x28: PLP
    opcodeTable[0x28] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = PLP }
        },
        .length = 1,
        .cycles = 4,
        .mode = IMPLIED
    };
    
    // Opcode 0x29: AND Immediate
    opcodeTable[0x29] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = AND }
        },
        .length = 2,
        .cycles = 2,
        .mode = IMMEDIATE
    };
    
    // Opcode 0x2A: ROL Accumulator
    opcodeTable[0x2A] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ROL }
        },
        .length = 1,
        .cycles = 2,
        .mode = ACCUMULATOR
    };
    
    // Opcode 0x2B: Undefined
    
    // Opcode 0x2C: BIT Absolute
    opcodeTable[0x2C] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .uchar_func = BIT }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x2D: AND Absolute
    opcodeTable[0x2D] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = AND }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x2E: ROL Absolute
    opcodeTable[0x2E] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ROL }
        },
        .length = 3,
        .cycles = 6,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x2F: Undefined
    
    // Opcode 0x30: BMI
    opcodeTable[0x30] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = BMI }
        },
        .length = 2,
        .cycles = 2,
        .mode = RELATIVE
    };
    
    // Opcode 0x31: AND (Indirect),Y
    opcodeTable[0x31] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = AND }
        },
        .length = 2,
        .cycles = 5,
        .mode = INDIRECT_Y
    };
    
    // Opcode 0x32: Undefined
    
    // Opcode 0x33: Undefined
    
    // Opcode 0x34: Undefined
    
    // Opcode 0x35: AND Zero Page,X
    opcodeTable[0x35] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = AND }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0x36: ROL Zero Page,X
    opcodeTable[0x36] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ROL }
        },
        .length = 2,
        .cycles = 6,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0x37: Undefined
    
    // Opcode 0x38: SEC
    opcodeTable[0x38] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = SEC }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0x39: AND Absolute,Y
    opcodeTable[0x39] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = AND }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_Y
    };
    
    // Opcode 0x3A: Undefined
    
    // Opcode 0x3B: Undefined
    
    // Opcode 0x3C: Undefined
    
    // Opcode 0x3D: AND Absolute,X
    opcodeTable[0x3D] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = AND }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0x3E: ROL Absolute,X
    opcodeTable[0x3E] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ROL }
        },
        .length = 3,
        .cycles = 7,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0x3F: Undefined
    
    // Opcode 0x40: RTI
    opcodeTable[0x40] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = RTI }
        },
        .length = 1,
        .cycles = 6,
        .mode = IMPLIED
    };
    
    // Opcode 0x41: EOR (Indirect,X)
    opcodeTable[0x41] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = EOR }
        },
        .length = 2,
        .cycles = 6,
        .mode = INDIRECT_X
    };
    
    // Opcode 0x42: Undefined
    
    // Opcode 0x43: Undefined
    
    // Opcode 0x44: Undefined
    
    // Opcode 0x45: EOR Zero Page
    opcodeTable[0x45] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = EOR }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x46: LSR Zero Page
    opcodeTable[0x46] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = LSR }
        },
        .length = 2,
        .cycles = 5,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x47: Undefined
    
    // Opcode 0x48: PHA
    opcodeTable[0x48] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = PHA }
        },
        .length = 1,
        .cycles = 3,
        .mode = IMPLIED
    };
    
    // Opcode 0x49: EOR Immediate
    opcodeTable[0x49] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = EOR }
        },
        .length = 2,
        .cycles = 2,
        .mode = IMMEDIATE
    };
    
    // Opcode 0x4A: LSR Accumulator
    opcodeTable[0x4A] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = LSR }
        },
        .length = 1,
        .cycles = 2,
        .mode = ACCUMULATOR
    };
    
    // Opcode 0x4B: Undefined
    
    // Opcode 0x4C: JMP Absolute
    opcodeTable[0x4C] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = JMP }
        },
        .length = 3,
        .cycles = 3,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x4D: EOR Absolute
    opcodeTable[0x4D] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .uchar_func = EOR }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x4E: LSR Absolute
    opcodeTable[0x4E] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = LSR }
        },
        .length = 3,
        .cycles = 6,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x4F: Undefined
    
    // Opcode 0x50: BVC
    opcodeTable[0x50] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = BVC }
        },
        .length = 2,
        .cycles = 2,
        .mode = RELATIVE
    };
    
    // Opcode 0x51: EOR (Indirect),Y
    opcodeTable[0x51] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = EOR }
        },
        .length = 2,
        .cycles = 5,
        .mode = INDIRECT_Y
    };
    
    // Opcode 0x52: Undefined
    
    // Opcode 0x53: Undefined
    
    // Opcode 0x54: Undefined
    
    // Opcode 0x55: EOR Zero Page,X
    opcodeTable[0x55] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = EOR }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0x56: LSR Zero Page,X
    opcodeTable[0x56] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = LSR }
        },
        .length = 2,
        .cycles = 6,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0x57: Undefined
    
    // Opcode 0x58: CLI
    opcodeTable[0x58] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = CLI }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0x59: EOR Absolute,Y
    opcodeTable[0x59] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .uchar_func = EOR }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_Y
    };
    
    // Opcode 0x5A: Undefined
    
    // Opcode 0x5B: Undefined
    
    // Opcode 0x5C: Undefined
    
    // Opcode 0x5D: EOR Absolute,X
    opcodeTable[0x5D] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .uchar_func = EOR }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0x5E: LSR Absolute,X
    opcodeTable[0x5E] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = LSR }
        },
        .length = 3,
        .cycles = 7,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0x5F: Undefined
    
    // Opcode 0x60: RTS
    opcodeTable[0x60] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = RTS }
        },
        .length = 1,
        .cycles = 6,
        .mode = IMPLIED
    };
    
    // Opcode 0x61: ADC (Indirect,X)
    opcodeTable[0x61] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ADC }
        },
        .length = 2,
        .cycles = 6,
        .mode = INDIRECT_X
    };
    
    // Opcode 0x62: Undefined
    
    // Opcode 0x63: Undefined
    
    // Opcode 0x64: Undefined
    
    // Opcode 0x65: ADC Zero Page
    opcodeTable[0x65] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ADC }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x66: ROR Zero Page
    opcodeTable[0x66] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ROR }
        },
        .length = 2,
        .cycles = 5,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x67: Undefined
    
    // Opcode 0x68: PLA
    opcodeTable[0x68] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = PLA }
        },
        .length = 1,
        .cycles = 4,
        .mode = IMPLIED
    };
    
    // Opcode 0x69: ADC Immediate
    opcodeTable[0x69] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ADC }
        },
        .length = 2,
        .cycles = 2,
        .mode = IMMEDIATE
    };
    
    // Opcode 0x6A: ROR Accumulator
    opcodeTable[0x6A] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ROR }
        },
        .length = 1,
        .cycles = 2,
        .mode = ACCUMULATOR
    };
    
    // Opcode 0x6B: Undefined
    
    // Opcode 0x6C: JMP Indirect
    opcodeTable[0x6C] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = JMP }
        },
        .length = 3,
        .cycles = 5,
        .mode = INDIRECT
    };
    
    // Opcode 0x6D: ADC Absolute
    opcodeTable[0x6D] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ADC }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x6E: ROR Absolute
    opcodeTable[0x6E] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ROR }
        },
        .length = 3,
        .cycles = 6,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x6F: Undefined
    
    // Opcode 0x70: BVS
    opcodeTable[0x70] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = BVS }
        },
        .length = 2,
        .cycles = 2,
        .mode = RELATIVE
    };
    
    // Opcode 0x71: ADC (Indirect),Y
    opcodeTable[0x71] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ADC }
        },
        .length = 2,
        .cycles = 5,
        .mode = INDIRECT_Y
    };
    
    // Opcode 0x72: Undefined
    
    // Opcode 0x73: Undefined
    
    // Opcode 0x74: Undefined
    
    // Opcode 0x75: ADC Zero Page,X
    opcodeTable[0x75] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ADC }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0x76: ROR Zero Page,X
    opcodeTable[0x76] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ROR }
        },
        .length = 2,
        .cycles = 6,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0x77: Undefined
    
    // Opcode 0x78: SEI
    opcodeTable[0x78] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = SEI }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0x79: ADC Absolute,Y
    opcodeTable[0x79] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ADC }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_Y
    };
    
    // Opcode 0x7A: Undefined
    
    // Opcode 0x7B: Undefined
    
    // Opcode 0x7C: Undefined
    
    // Opcode 0x7D: ADC Absolute,X
    opcodeTable[0x7D] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = ADC }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0x7E: ROR Absolute,X
    opcodeTable[0x7E] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR_PTR,
            .func = { .uchar_ptr_func = ROR }
        },
        .length = 3,
        .cycles = 7,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0x7F: Undefined
    
    // Opcode 0x80: Undefined
    
    // Opcode 0x81: STA (Indirect,X)
    opcodeTable[0x81] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STA }
        },
        .length = 2,
        .cycles = 6,
        .mode = INDIRECT_X
    };
    
    // Opcode 0x82: Undefined
    
    // Opcode 0x83: Undefined
    
    // Opcode 0x84: STY Zero Page
    opcodeTable[0x84] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STY }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x85: STA Zero Page
    opcodeTable[0x85] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STA }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x86: STX Zero Page
    opcodeTable[0x86] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STX }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0x87: Undefined
    
    // Opcode 0x88: DEY
    opcodeTable[0x88] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = DEY }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0x89: Undefined
    
    // Opcode 0x8A: TXA
    opcodeTable[0x8A] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = TXA }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0x8B: Undefined
    
    // Opcode 0x8C: STY Absolute
    opcodeTable[0x8C] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STY }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x8D: STA Absolute
    opcodeTable[0x8D] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STA }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x8E: STX Absolute
    opcodeTable[0x8E] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STX }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0x8F: Undefined
    
    // Opcode 0x90: BCC
    opcodeTable[0x90] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = BCC }
        },
        .length = 2,
        .cycles = 2,
        .mode = RELATIVE
    };
    
    // Opcode 0x91: STA (Indirect),Y
    opcodeTable[0x91] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STA }
        },
        .length = 2,
        .cycles = 6,
        .mode = INDIRECT_Y
    };
    
    // Opcode 0x92: Undefined
    
    // Opcode 0x93: Undefined
    
    // Opcode 0x94: STY Zero Page,X
    opcodeTable[0x94] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STY }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0x95: STA Zero Page,X
    opcodeTable[0x95] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STA }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0x96: STX Zero Page,Y
    opcodeTable[0x96] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STX }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_Y
    };
    
    // Opcode 0x97: Undefined
    
    // Opcode 0x98: TYA
    opcodeTable[0x98] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = TYA }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0x99: STA Absolute,Y
    opcodeTable[0x99] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STA }
        },
        .length = 3,
        .cycles = 5,
        .mode = ABSOLUTE_Y
    };
    
    // Opcode 0x9A: TXS
    opcodeTable[0x9A] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = TXS }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0x9B: Undefined
    
    // Opcode 0x9C: Undefined
    
    // Opcode 0x9D: STA Absolute,X
    opcodeTable[0x9D] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = STA }
        },
        .length = 3,
        .cycles = 5,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0x9E: Undefined
    
    // Opcode 0x9F: Undefined
    
    // Opcode 0xA0: LDY Immediate
    opcodeTable[0xA0] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDY }
        },
        .length = 2,
        .cycles = 2,
        .mode = IMMEDIATE
    };
    
    // Opcode 0xA1: LDA (Indirect,X)
    opcodeTable[0xA1] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDA }
        },
        .length = 2,
        .cycles = 6,
        .mode = INDIRECT_X
    };
    
    // Opcode 0xA2: LDX Immediate
    opcodeTable[0xA2] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDX }
        },
        .length = 2,
        .cycles = 2,
        .mode = IMMEDIATE
    };
    
    // Opcode 0xA3: Undefined
    
    // Opcode 0xA4: LDY Zero Page
    opcodeTable[0xA4] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDY }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0xA5: LDA Zero Page
    opcodeTable[0xA5] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDA }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0xA6: LDX Zero Page
    opcodeTable[0xA6] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDX }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0xA7: Undefined
    
    // Opcode 0xA8: TAY
    opcodeTable[0xA8] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = TAY }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0xA9: LDA Immediate
    opcodeTable[0xA9] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDA }
        },
        .length = 2,
        .cycles = 2,
        .mode = IMMEDIATE
    };
    
    // Opcode 0xAA: TAX
    opcodeTable[0xAA] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = TAX }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0xAB: Undefined
    
    // Opcode 0xAC: LDY Absolute
    opcodeTable[0xAC] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDY }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0xAD: LDA Absolute
    opcodeTable[0xAD] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDA }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0xAE: LDX Absolute
    opcodeTable[0xAE] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDX }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0xAF: Undefined
    
    // Opcode 0xB0: BCS
    opcodeTable[0xB0] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = BCS }
        },
        .length = 2,
        .cycles = 2,
        .mode = RELATIVE
    };
    
    // Opcode 0xB1: LDA (Indirect),Y
    opcodeTable[0xB1] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDA }
        },
        .length = 2,
        .cycles = 5,
        .mode = INDIRECT_Y
    };
    
    // Opcode 0xB2: Undefined
    
    // Opcode 0xB3: Undefined
    
    // Opcode 0xB4: LDY Zero Page,X
    opcodeTable[0xB4] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDY }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0xB5: LDA Zero Page,X
    opcodeTable[0xB5] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDA }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0xB6: LDX Zero Page,Y
    opcodeTable[0xB6] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDX }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_Y
    };
    
    // Opcode 0xB7: Undefined
    
    // Opcode 0xB8: CLV
    opcodeTable[0xB8] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = CLV }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0xB9: LDA Absolute,Y
    opcodeTable[0xB9] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDA }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_Y
    };
    
    // Opcode 0xBA: TSX
    opcodeTable[0xBA] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = TSX }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0xBB: Undefined
    
    // Opcode 0xBC: LDY Absolute,X
    opcodeTable[0xBC] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDY }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0xBD: LDA Absolute,X
    opcodeTable[0xBD] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDA }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0xBE: LDX Absolute,Y
    opcodeTable[0xBE] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = LDX }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_Y
    };
    
    // Opcode 0xBF: Undefined
    
    // Opcode 0xC0: CPY Immediate
    opcodeTable[0xC0] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CPY }
        },
        .length = 2,
        .cycles = 2,
        .mode = IMMEDIATE
    };
    
    // Opcode 0xC1: CMP (Indirect,X)
    opcodeTable[0xC1] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CMP }
        },
        .length = 2,
        .cycles = 6,
        .mode = INDIRECT_X
    };
    
    // Opcode 0xC2: Undefined
    
    // Opcode 0xC3: Undefined
    
    // Opcode 0xC4: CPY Zero Page
    opcodeTable[0xC4] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CPY }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0xC5: CMP Zero Page
    opcodeTable[0xC5] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CMP }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0xC6: DEC Zero Page
    opcodeTable[0xC6] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = DEC }
        },
        .length = 2,
        .cycles = 5,
        .mode = ZEROPAGE
    };
    
    // Opcode 0xC7: Undefined
    
    // Opcode 0xC8: INY
    opcodeTable[0xC8] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = INY }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0xC9: CMP Immediate
    opcodeTable[0xC9] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CMP }
        },
        .length = 2,
        .cycles = 2,
        .mode = IMMEDIATE
    };
    
    // Opcode 0xCA: DEX
    opcodeTable[0xCA] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = DEX }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0xCB: Undefined
    
    // Opcode 0xCC: CPY Absolute
    opcodeTable[0xCC] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CPY }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0xCD: CMP Absolute
    opcodeTable[0xCD] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CMP }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0xCE: DEC Absolute
    opcodeTable[0xCE] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = DEC }
        },
        .length = 3,
        .cycles = 6,
        .mode = ABSOLUTE
    };
    
    // Opcode 0xCF: Undefined
    
    // Opcode 0xD0: BNE
    opcodeTable[0xD0] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = BNE }
        },
        .length = 2,
        .cycles = 2,
        .mode = RELATIVE
    };
    
    // Opcode 0xD1: CMP (Indirect),Y
    opcodeTable[0xD1] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CMP }
        },
        .length = 2,
        .cycles = 5,
        .mode = INDIRECT_Y
    };
    
    // Opcode 0xD2: Undefined
    
    // Opcode 0xD3: Undefined
    
    // Opcode 0xD4: Undefined
    
    // Opcode 0xD5: CMP Zero Page,X
    opcodeTable[0xD5] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CMP }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0xD6: DEC Zero Page,X
    opcodeTable[0xD6] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = DEC }
        },
        .length = 2,
        .cycles = 6,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0xD7: Undefined
    
    // Opcode 0xD8: CLD
    opcodeTable[0xD8] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = CLD }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0xD9: CMP Absolute,Y
    opcodeTable[0xD9] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CMP }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_Y
    };
    
    // Opcode 0xDA: Undefined
    
    // Opcode 0xDB: Undefined
    
    // Opcode 0xDC: Undefined
    
    // Opcode 0xDD: CMP Absolute,X
    opcodeTable[0xDD] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CMP }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0xDE: DEC Absolute,X
    opcodeTable[0xDE] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = DEC }
        },
        .length = 3,
        .cycles = 7,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0xDF: Undefined
    
    // Opcode 0xE0: CPX Immediate
    opcodeTable[0xE0] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CPX }
        },
        .length = 2,
        .cycles = 2,
        .mode = IMMEDIATE
    };
    
    // Opcode 0xE1: SBC (Indirect,X)
    opcodeTable[0xE1] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = SBC }
        },
        .length = 2,
        .cycles = 6,
        .mode = INDIRECT_X
    };
    
    // Opcode 0xE2: Undefined
    
    // Opcode 0xE3: Undefined
    
    // Opcode 0xE4: CPX Zero Page
    opcodeTable[0xE4] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CPX }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0xE5: SBC Zero Page
    opcodeTable[0xE5] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = SBC }
        },
        .length = 2,
        .cycles = 3,
        .mode = ZEROPAGE
    };
    
    // Opcode 0xE6: INC Zero Page
    opcodeTable[0xE6] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = INC }
        },
        .length = 2,
        .cycles = 5,
        .mode = ZEROPAGE
    };
    
    // Opcode 0xE7: Undefined
    
    // Opcode 0xE8: INX
    opcodeTable[0xE8] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = INX }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0xE9: SBC Immediate
    opcodeTable[0xE9] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = SBC }
        },
        .length = 2,
        .cycles = 2,
        .mode = IMMEDIATE
    };
    
    // Opcode 0xEA: NOP
    opcodeTable[0xEA] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = NOP }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0xEB: Undefined
    
    // Opcode 0xEC: CPX Absolute
    opcodeTable[0xEC] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = CPX }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0xED: SBC Absolute
    opcodeTable[0xED] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = SBC }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE
    };
    
    // Opcode 0xEE: INC Absolute
    opcodeTable[0xEE] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = INC }
        },
        .length = 3,
        .cycles = 6,
        .mode = ABSOLUTE
    };
    
    // Opcode 0xEF: Undefined
    
    // Opcode 0xF0: BEQ
    opcodeTable[0xF0] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = BEQ }
        },
        .length = 2,
        .cycles = 2,
        .mode = RELATIVE
    };
    
    // Opcode 0xF1: SBC (Indirect),Y
    opcodeTable[0xF1] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = SBC }
        },
        .length = 2,
        .cycles = 5,
        .mode = INDIRECT_Y
    };
    
    // Opcode 0xF2: Undefined
    
    // Opcode 0xF3: Undefined
    
    // Opcode 0xF4: Undefined
    
    // Opcode 0xF5: SBC Zero Page,X
    opcodeTable[0xF5] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = SBC }
        },
        .length = 2,
        .cycles = 4,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0xF6: INC Zero Page,X
    opcodeTable[0xF6] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = INC }
        },
        .length = 2,
        .cycles = 6,
        .mode = ZEROPAGE_X
    };
    
    // Opcode 0xF7: Undefined
    
    // Opcode 0xF8: SED
    opcodeTable[0xF8] = {
        .handler = {
            .type = InstructionHandler::TYPE_VOID,
            .func = { .void_func = SED }
        },
        .length = 1,
        .cycles = 2,
        .mode = IMPLIED
    };
    
    // Opcode 0xF9: SBC Absolute,Y
    opcodeTable[0xF9] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = SBC }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_Y
    };
    
    // Opcode 0xFA: Undefined
    
    // Opcode 0xFB: Undefined
    
    // Opcode 0xFC: Undefined
    
    // Opcode 0xFD: SBC Absolute,X
    opcodeTable[0xFD] = {
        .handler = {
            .type = InstructionHandler::TYPE_UCHAR,
            .func = { .uchar_func = SBC }
        },
        .length = 3,
        .cycles = 4,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0xFE: INC Absolute,X
    opcodeTable[0xFE] = {
        .handler = {
            .type = InstructionHandler::TYPE_USHORT,
            .func = { .ushort_func = INC }
        },
        .length = 3,
        .cycles = 7,
        .mode = ABSOLUTE_X
    };
    
    // Opcode 0xFF: Undefined
}