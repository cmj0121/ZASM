/* Copyright (C) 2017-2019 cmj. All right reserved. */
#pragma once

{0x00, "add",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_REG_FIELD},
{0x03, "add",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_REG_FIELD},
{0x01, "add",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_REG_FIELD},
{0x02, "add",		CODE_R_SIZE8,			CODE_RM_SIZE8,			CODE_REG_FIELD},
{0x04, "add",		CODE_AL,				CODE_I_SIZE8,			CODE_NONE},
{0x05, "add",		CODE_EAX,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
{0x05, "add",		CODE_RAX,				CODE_I_SIZE16_32,		CODE_NONE},
{0x06, "push",		CODE_R_ES,				CODE_NONE,				CODE_X86_ONLY},
{0x07, "pop",		CODE_R_ES,				CODE_NONE,				CODE_X86_ONLY},
{0x08, "or",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_REG_FIELD},
{0x0B, "or",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_REG_FIELD},
{0x09, "or",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_REG_FIELD},
{0x0A, "or",		CODE_R_SIZE8,			CODE_RM_SIZE8,			CODE_REG_FIELD},
{0x0C, "or",		CODE_AL,				CODE_I_SIZE8,			CODE_NONE},
{0x0D, "or",		CODE_EAX,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
{0x0E, "push",		CODE_R_CS,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
/* 0x0F - Two-byte Instruction */
{0x10, "adc",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_REG_FIELD},
{0x13, "adc",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_REG_FIELD},
{0x11, "adc",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_REG_FIELD},
{0x12, "adc",		CODE_R_SIZE8,			CODE_RM_SIZE8,			CODE_REG_FIELD},
{0x14, "adc",		CODE_AL,				CODE_I_SIZE8,			CODE_NONE},
{0x15, "adc",		CODE_EAX,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
{0x15, "adc",		CODE_RAX,				CODE_I_SIZE16_32,		CODE_NONE},
{0x16, "push",		CODE_R_SS,				CODE_NONE,				CODE_X86_ONLY},
{0x17, "pop",		CODE_R_SS,				CODE_NONE,				CODE_X86_ONLY},
{0x18, "sbb",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_REG_FIELD},
{0x1B, "sbb",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_REG_FIELD},
{0x19, "sbb",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_REG_FIELD},
{0x1A, "sbb",		CODE_R_SIZE8,			CODE_RM_SIZE8,			CODE_REG_FIELD},
{0x1C, "sbb",		CODE_AL,				CODE_I_SIZE8,			CODE_NONE},
{0x1D, "sbb",		CODE_EAX,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
{0x1E, "push",		CODE_R_DS,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
{0x1E, "pop",		CODE_R_DS,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
{0x20, "and",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_REG_FIELD},
{0x21, "and",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_REG_FIELD},
{0x22, "and",		CODE_R_SIZE8,			CODE_RM_SIZE8,			CODE_REG_FIELD},
{0x23, "and",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_REG_FIELD},
{0x24, "and",		CODE_AL,				CODE_I_SIZE8,			CODE_NONE},
{0x25, "and",		CODE_EAX,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
{0x25, "and",		CODE_RAX,				CODE_I_SIZE16_32,		CODE_NONE},
/* 0x26 - ES prefix */
{0x27, "daa",		CODE_AL,				CODE_NONE,				CODE_X86_ONLY},
{0x28, "sub",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_REG_FIELD},
{0x29, "sub",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_REG_FIELD},
{0x2A, "sub",		CODE_R_SIZE8,			CODE_RM_SIZE8,			CODE_REG_FIELD},
{0x2B, "sub",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_REG_FIELD},
{0x2C, "sub",		CODE_AL,				CODE_I_SIZE8,			CODE_NONE},
{0x2D, "sub",		CODE_EAX,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
{0x2E, "push",		CODE_R_CS,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
/* 0x2E - CS prefix */
{0x30, "xor",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_REG_FIELD},
{0x31, "xor",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_REG_FIELD},
{0x32, "xor",		CODE_R_SIZE8,			CODE_RM_SIZE8,			CODE_REG_FIELD},
{0x33, "xor",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_REG_FIELD},
{0x34, "xor",		CODE_AL,				CODE_I_SIZE8,			CODE_NONE},
{0x35, "xor",		CODE_EAX,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
{0x35, "xor",		CODE_RAX,				CODE_I_SIZE16_32,		CODE_NONE},
/* 0x36 - SS prefix */
{0x37, "aaa",		CODE_AL,				CODE_AH,				CODE_X86_ONLY},
{0x38, "cmp",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_REG_FIELD},
{0x39, "cmp",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_REG_FIELD},
{0x3A, "cmp",		CODE_R_SIZE8,			CODE_RM_SIZE8,			CODE_REG_FIELD},
{0x3B, "cmp",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_REG_FIELD},
{0x3C, "cmp",		CODE_AL,				CODE_I_SIZE8,			CODE_NONE},
{0x3D, "cmp",		CODE_EAX,				CODE_I_SIZE16_32,		CODE_X86_ONLY},
/* 0x3E - DS prefix */
{0x3F, "aas",		CODE_AL,				CODE_AH,				CODE_X86_ONLY},
{0x40, "inc",		CODE_R_SIZE16_32,		CODE_NONE,				CODE_X86_ONLY | CODE_SECOND_OPCODE},
{0x48, "dec",		CODE_R_SIZE16_32,		CODE_NONE,				CODE_X86_ONLY | CODE_SECOND_OPCODE},
{0x50, "push",		CODE_R_SIZE16_64,		CODE_NONE,				CODE_SECOND_OPCODE},
{0x50, "push",		CODE_R_SIZE16_32,		CODE_NONE,				CODE_X86_ONLY | CODE_SECOND_OPCODE},
{0x58, "pop",		CODE_R_SIZE16_64,		CODE_NONE,				CODE_SECOND_OPCODE},
{0x58, "pop",		CODE_R_SIZE16_32,		CODE_NONE,				CODE_X86_ONLY | CODE_SECOND_OPCODE},
{0x60, "pusha",		CODE_NONE,				CODE_NONE,				CODE_X86_ONLY},
{0x60, "pushad",	CODE_NONE,				CODE_NONE,				CODE_X86_ONLY},
{0x61, "popa",		CODE_NONE,				CODE_NONE,				CODE_X86_ONLY},
{0x61, "popad",		CODE_NONE,				CODE_NONE,				CODE_X86_ONLY},
/* 0x62 */
/* 0x63 */
/* 0x64 - FS prefix */
/* 0x65 - GS prefix */
/* 0x66 - Operand-size override prefix */
/* 0x67 - Address-size override prefix */
{0x68, "push",		CODE_I_SIZE16_32,		CODE_NONE,				CODE_NONE},
{0x69, "imul",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32,		CODE_REG_FIELD},
{0x6A, "push",		CODE_RM_SIZE8,			CODE_NONE,				CODE_NONE},
{0x6B, "imul",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_REG_FIELD},
{0x6C, "ins",		CODE_M_SIZE8,			CODE_NONE,				CODE_NONE},
{0x6D, "ins",		CODE_M_SIZE16_32,		CODE_NONE,				CODE_NONE},
{0x6E, "outs",		CODE_M_SIZE8,			CODE_NONE,				CODE_NONE},
{0x6F, "outs",		CODE_M_SIZE16_32,		CODE_NONE,				CODE_NONE},
{0x70, "jo",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x71, "jno",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x72, "jc",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x73, "jnc",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x74, "je",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x74, "jz",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x75, "jne",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x75, "jnz",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x76, "jbe",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x77, "jnbe",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x78, "js",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x79, "jns",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x7A, "jp",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x7B, "jnp",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x7C, "jl",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x7D, "jnl",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x7D, "jge",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x7E, "jle",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x7F, "jg",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0x80, "add",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_00},
{0x80, "or",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_01},
{0x80, "adc",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_02},
{0x80, "sbb",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_03},
{0x80, "and",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_04},
{0x80, "sub",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_05},
{0x80, "xor",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_06},
{0x80, "cmp",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_07},
{0x81, "add",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32,		CODE_OP_FIELD_00},
{0x81, "or",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32,		CODE_OP_FIELD_01},
{0x81, "adc",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32,		CODE_OP_FIELD_02},
{0x81, "sbb",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32,		CODE_OP_FIELD_03},
{0x81, "and",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32,		CODE_OP_FIELD_04},
{0x81, "sub",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32,		CODE_OP_FIELD_05},
{0x81, "xor",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32,		CODE_OP_FIELD_06},
{0x81, "cmp",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32,		CODE_OP_FIELD_07},
/* 0x82 - invalid */
{0x83, "add",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_OP_FIELD_00},
{0x83, "or",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_OP_FIELD_01},
{0x83, "adc",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_OP_FIELD_02},
{0x83, "sbb",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_OP_FIELD_03},
{0x83, "and",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_OP_FIELD_04},
{0x83, "sub",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_OP_FIELD_05},
{0x83, "xor",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_OP_FIELD_06},
{0x83, "cmp",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_OP_FIELD_07},
{0x84, "test",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_REG_FIELD},
{0x85, "test",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_REG_FIELD},
{0x86, "xchg",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_REG_FIELD},
{0x87, "xchg",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_REG_FIELD},
{0x88, "mov",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_REG_FIELD},
{0x8B, "mov",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_REG_FIELD},
{0x89, "mov",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_REG_FIELD},
{0x8A, "mov",		CODE_R_SIZE8,			CODE_RM_SIZE8,			CODE_REG_FIELD},
{0x8C, "mov",		CODE_R_SIZE16_32_64,	CODE_REG_SEG,			CODE_REG_FIELD},
{0x8C, "mov",		CODE_M_SIZE16,			CODE_REG_SEG,			CODE_REG_FIELD},
{0x8D, "lea",		CODE_R_SIZE16_32_64,	CODE_M_SIZE32_64,		CODE_REG_FIELD},
{0x8E, "mov",		CODE_REG_SEG,			CODE_RM_SIZE16,			CODE_REG_FIELD},
{0x8F, "pop",		CODE_RM_SIZE16_32,		CODE_NONE,				CODE_OP_FIELD_00},
{0x90, "nop",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0x90, "xchg",		CODE_R_SIZE16_32_64,	CODE_RAX,				CODE_REG_FIELD},
{0x90, "pause",		CODE_NONE,				CODE_NONE,				CODE_PRE_SP},
{0x98, "cwb",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0x99, "cwd",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0x9A, "call",		CODE_ADDRESS_OFF,		CODE_NONE,				CODE_X86_ONLY},
{0x9B, "wait",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0x9C, "pushf",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0x9D, "popf",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0x9E, "sahf",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0x9F, "lahf",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xA0, "mov",		CODE_AL,				CODE_SEG_OFF_SIZE8,			CODE_NONE},
{0xA1, "mov",		CODE_RAX,				CODE_SEG_OFF_SIZE16_32_64,	CODE_NONE},
{0xA2, "mov",		CODE_SEG_OFF_SIZE8,		CODE_AL,					CODE_NONE},
{0xA3, "mov",		CODE_SEG_OFF_SIZE16_32_64,	CODE_AL,				CODE_NONE},
{0xA4, "movsb",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xA5, "movsw",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xA5, "movsd",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xA5, "movsq",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xA6, "cmpsb",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xA7, "cmpsw",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xA7, "cmpsd",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xA7, "cmpsq",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xA8, "test",		CODE_AL,				CODE_I_SIZE8,			CODE_NONE},
{0xA9, "test",		CODE_RAX,				CODE_I_SIZE16_32,		CODE_NONE},
{0xAA, "stosb",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xAB, "stosw",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xAB, "stosd",		CODE_NONE,				CODE_NONE,				CODE_PRE_OPERAND},
{0xAB, "stosq",		CODE_NONE,				CODE_NONE,				CODE_FORCE_64BIT},
{0xAC, "lodsb",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xAD, "lodsw",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xAD, "lodsd",		CODE_NONE,				CODE_NONE,				CODE_PRE_OPERAND},
{0xAD, "lodsq",		CODE_NONE,				CODE_NONE,				CODE_FORCE_64BIT},
{0xAE, "scas",		CODE_M_SIZE8,			CODE_AL,				CODE_NONE},
{0xAF, "scas",		CODE_M_SIZE16_32_64,	CODE_AL,				CODE_NONE},
{0xB0, "mov",		CODE_R_SIZE8,			CODE_I_SIZE8,			CODE_SECOND_OPCODE},
{0xB8, "mov",		CODE_R_SIZE16_32_64,	CODE_I_SIZE16_32_64,	CODE_SECOND_OPCODE},
{0xC0, "rol",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_00},
{0xC0, "ror",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_01},
{0xC0, "rcl",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_02},
{0xC0, "rcr",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_03},
{0xC0, "shl",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_04},
{0xC0, "shr",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_05},
{0xC0, "sal",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_06},
{0xC0, "sar",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_07},
{0xC1, "rol",		CODE_RM_SIZE16_32,		CODE_I_SIZE8,			CODE_OP_FIELD_00},
{0xC1, "ror",		CODE_RM_SIZE16_32,		CODE_I_SIZE8,			CODE_OP_FIELD_01},
{0xC1, "rcl",		CODE_RM_SIZE16_32,		CODE_I_SIZE8,			CODE_OP_FIELD_02},
{0xC1, "rcr",		CODE_RM_SIZE16_32,		CODE_I_SIZE8,			CODE_OP_FIELD_03},
{0xC1, "shl",		CODE_RM_SIZE16_32,		CODE_I_SIZE8,			CODE_OP_FIELD_04},
{0xC1, "shr",		CODE_RM_SIZE16_32,		CODE_I_SIZE8,			CODE_OP_FIELD_05},
{0xC1, "sal",		CODE_RM_SIZE16_32,		CODE_I_SIZE8,			CODE_OP_FIELD_06},
{0xC1, "sar",		CODE_RM_SIZE16_32,		CODE_I_SIZE8,			CODE_OP_FIELD_07},
{0xC2, "ret",		CODE_I_SIZE16,			CODE_NONE,				CODE_NONE},
{0xC3, "ret",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xC4, "les",		CODE_R_SIZE16_32,		CODE_ADDRESS_OFF,		CODE_REG_FIELD},
{0xC5, "lds",		CODE_R_SIZE16_32,		CODE_ADDRESS_OFF,		CODE_REG_FIELD},
{0xC6, "mov",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_00},
{0xC7, "mov",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32_64,	CODE_OP_FIELD_00},
{0xC8, "enter",		CODE_I_SIZE16,			CODE_I_SIZE8,			CODE_NONE},
{0xC9, "leave",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xCA, "retf",		CODE_I_SIZE16,			CODE_NONE,				CODE_NONE},
{0xCB, "retf",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xCC, "int",		CODE_CONSTANT_3,		CODE_NONE,				CODE_NONE},
{0xCC, "int3",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xCD, "int",		CODE_I_SIZE8,			CODE_NONE,				CODE_NONE},
{0xCE, "into",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xCF, "iret",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xD0, "rol",		CODE_RM_SIZE8,			CODE_CONSTANT_1,		CODE_OP_FIELD_00},
{0xD0, "ror",		CODE_RM_SIZE8,			CODE_CONSTANT_1,		CODE_OP_FIELD_01},
{0xD0, "rcl",		CODE_RM_SIZE8,			CODE_CONSTANT_1,		CODE_OP_FIELD_02},
{0xD0, "rcr",		CODE_RM_SIZE8,			CODE_CONSTANT_1,		CODE_OP_FIELD_03},
{0xD0, "shl",		CODE_RM_SIZE8,			CODE_CONSTANT_1,		CODE_OP_FIELD_04},
{0xD0, "shr",		CODE_RM_SIZE8,			CODE_CONSTANT_1,		CODE_OP_FIELD_05},
{0xD0, "sal",		CODE_RM_SIZE8,			CODE_CONSTANT_1,		CODE_OP_FIELD_06},
{0xD0, "sar",		CODE_RM_SIZE8,			CODE_CONSTANT_1,		CODE_OP_FIELD_07},
{0xD1, "rol",		CODE_RM_SIZE16_32,		CODE_CONSTANT_1,		CODE_OP_FIELD_00},
{0xD1, "ror",		CODE_RM_SIZE16_32,		CODE_CONSTANT_1,		CODE_OP_FIELD_01},
{0xD1, "rcl",		CODE_RM_SIZE16_32,		CODE_CONSTANT_1,		CODE_OP_FIELD_02},
{0xD1, "rcr",		CODE_RM_SIZE16_32,		CODE_CONSTANT_1,		CODE_OP_FIELD_03},
{0xD1, "shl",		CODE_RM_SIZE16_32,		CODE_CONSTANT_1,		CODE_OP_FIELD_04},
{0xD1, "shr",		CODE_RM_SIZE16_32,		CODE_CONSTANT_1,		CODE_OP_FIELD_05},
{0xD1, "sal",		CODE_RM_SIZE16_32,		CODE_CONSTANT_1,		CODE_OP_FIELD_06},
{0xD1, "sar",		CODE_RM_SIZE16_32,		CODE_CONSTANT_1,		CODE_OP_FIELD_07},
{0xD2, "rol",		CODE_RM_SIZE8,			CODE_CL,				CODE_OP_FIELD_00},
{0xD2, "ror",		CODE_RM_SIZE8,			CODE_CL,				CODE_OP_FIELD_01},
{0xD2, "rcl",		CODE_RM_SIZE8,			CODE_CL,				CODE_OP_FIELD_02},
{0xD2, "rcr",		CODE_RM_SIZE8,			CODE_CL,				CODE_OP_FIELD_03},
{0xD2, "shl",		CODE_RM_SIZE8,			CODE_CL,				CODE_OP_FIELD_04},
{0xD2, "shr",		CODE_RM_SIZE8,			CODE_CL,				CODE_OP_FIELD_05},
{0xD2, "sal",		CODE_RM_SIZE8,			CODE_CL,				CODE_OP_FIELD_06},
{0xD2, "sar",		CODE_RM_SIZE8,			CODE_CL,				CODE_OP_FIELD_07},
{0xD3, "rol",		CODE_RM_SIZE16_32,		CODE_CL,				CODE_OP_FIELD_00},
{0xD3, "ror",		CODE_RM_SIZE16_32,		CODE_CL,				CODE_OP_FIELD_01},
{0xD3, "rcl",		CODE_RM_SIZE16_32,		CODE_CL,				CODE_OP_FIELD_02},
{0xD3, "rcr",		CODE_RM_SIZE16_32,		CODE_CL,				CODE_OP_FIELD_03},
{0xD3, "shl",		CODE_RM_SIZE16_32,		CODE_CL,				CODE_OP_FIELD_04},
{0xD3, "shr",		CODE_RM_SIZE16_32,		CODE_CL,				CODE_OP_FIELD_05},
{0xD3, "sal",		CODE_RM_SIZE16_32,		CODE_CL,				CODE_OP_FIELD_06},
{0xD3, "sar",		CODE_RM_SIZE16_32,		CODE_CL,				CODE_OP_FIELD_07},
{0xD4, "aam",		CODE_NONE,				CODE_NONE,				CODE_X86_ONLY},
{0xD5, "aad",		CODE_NONE,				CODE_NONE,				CODE_X86_ONLY},
{0xD6, "salc",		CODE_NONE,				CODE_NONE,				CODE_X86_ONLY},
{0xD7, "xlat",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xD8, "fadd",		CODE_REG_ST,			CODE_R_ST_M32_REAL,		CODE_OP_FIELD_00},
{0xD8, "fmul",		CODE_REG_ST,			CODE_R_ST_M32_REAL,		CODE_OP_FIELD_01},
{0xD8, "fcom",		CODE_REG_ST,			CODE_R_ST_M32_REAL,		CODE_OP_FIELD_02},
{0xD8, "fcomp",		CODE_REG_ST,			CODE_R_ST_M32_REAL,		CODE_OP_FIELD_03},
{0xD8, "fsub",		CODE_REG_ST,			CODE_R_ST_M32_REAL,		CODE_OP_FIELD_04},
{0xD8, "fsubr",		CODE_REG_ST,			CODE_R_ST_M32_REAL,		CODE_OP_FIELD_05},
{0xD8, "fdiv",		CODE_REG_ST,			CODE_R_ST_M32_REAL,		CODE_OP_FIELD_06},
{0xD8, "fdivr",		CODE_REG_ST,			CODE_R_ST_M32_REAL,		CODE_OP_FIELD_07},
{0xD9, "fld",		CODE_REG_ST,			CODE_R_ST_M32_REAL,		CODE_OP_FIELD_00},
{0xD9, "fxchg",		CODE_REG_ST,			CODE_REG_ST,			CODE_OP_FIELD_01},
{0xD9, "fnop",		CODE_NONE,				CODE_NONE,				CODE_OP_FIELD_02},
{0xD9, "fstp",		CODE_R_ST_M32_REAL,		CODE_REG_ST,			CODE_OP_FIELD_03},
{0xE0, "loopnz",	CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0xE1, "loopz",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0xE2, "loop",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0xE3, "jecxz",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0xE4, "in",		CODE_AL,				CODE_I_SIZE8,			CODE_NONE},
{0xE5, "in",		CODE_EAX,				CODE_I_SIZE8,			CODE_NONE},
{0xE6, "out",		CODE_I_SIZE8,			CODE_AL,				CODE_NONE},
{0xE7, "out",		CODE_I_SIZE8,			CODE_EAX,				CODE_NONE},
{0xE8, "call",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_NONE},
{0xE9, "jmp",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_NONE},
{0xEA, "jmp",		CODE_ADDRESS_OFF,		CODE_NONE,				CODE_X86_ONLY},
{0xEB, "jmp",		CODE_REL_SIZE8,			CODE_NONE,				CODE_NONE},
{0xEC, "in",		CODE_AL,				CODE_DX,				CODE_NONE},
{0xED, "in",		CODE_EAX,				CODE_DX,				CODE_NONE},
{0xEE, "out",		CODE_DX,				CODE_AL,				CODE_NONE},
{0xEF, "out",		CODE_DX,				CODE_EAX,				CODE_NONE},
/* 0xF0 - LOCK prefix */
/* 0xF1 */
{0xF2, "repzn",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xF3, "rep",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xF4, "hlt",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xF5, "cmc",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xF6, "test",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_00},
{0xF6, "test",		CODE_RM_SIZE8,			CODE_I_SIZE8,			CODE_OP_FIELD_01},
{0xF6, "not",		CODE_RM_SIZE8,			CODE_NONE,				CODE_OP_FIELD_02},
{0xF6, "neg",		CODE_RM_SIZE8,			CODE_NONE,				CODE_OP_FIELD_03},
{0xF6, "mul",		CODE_AL,				CODE_RM_SIZE8,			CODE_OP_FIELD_04},
{0xF6, "imul",		CODE_AL,				CODE_RM_SIZE8,			CODE_OP_FIELD_05},
{0xF6, "div",		CODE_AX,				CODE_RM_SIZE8,			CODE_OP_FIELD_06},
{0xF6, "div",		CODE_RM_SIZE8,			CODE_NONE,				CODE_OP_FIELD_06},
{0xF6, "idiv",		CODE_AX,				CODE_RM_SIZE8,			CODE_OP_FIELD_07},
{0xF6, "idiv",		CODE_RM_SIZE8,			CODE_NONE,				CODE_OP_FIELD_07},
{0xF7, "test",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32,		CODE_OP_FIELD_00},
{0xF7, "test",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE16_32,		CODE_OP_FIELD_01},
{0xF7, "not",		CODE_RM_SIZE16_32_64,	CODE_NONE,				CODE_OP_FIELD_02},
{0xF7, "neg",		CODE_RM_SIZE16_32_64,	CODE_NONE,				CODE_OP_FIELD_03},
{0xF7, "mul",		CODE_EAX,				CODE_RM_SIZE16_32,		CODE_OP_FIELD_04},
{0xF7, "imul",		CODE_EAX,				CODE_RM_SIZE16_32,		CODE_OP_FIELD_05},
{0xF7, "div",		CODE_EAX,				CODE_RM_SIZE16_32,		CODE_OP_FIELD_06},
{0xF7, "div",		CODE_RM_SIZE16_32,		CODE_NONE,				CODE_OP_FIELD_06},
{0xF7, "idiv",		CODE_EAX,				CODE_RM_SIZE16_32,		CODE_OP_FIELD_07},
{0xF7, "idiv",		CODE_RM_SIZE16_32,		CODE_NONE,				CODE_OP_FIELD_07},
{0xF8, "clc",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xF9, "stc",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xFA, "cli",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xFB, "sti",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xFC, "cld",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xFD, "std",		CODE_NONE,				CODE_NONE,				CODE_NONE},
{0xFE, "inc",		CODE_RM_SIZE8,			CODE_NONE,				CODE_OP_FIELD_00},
{0xFE, "dec",		CODE_RM_SIZE8,			CODE_NONE,				CODE_OP_FIELD_01},
{0xFF, "inc",		CODE_RM_SIZE16_32_64,	CODE_NONE,				CODE_OP_FIELD_00},
{0xFF, "dec",		CODE_RM_SIZE16_32_64,	CODE_NONE,				CODE_OP_FIELD_01},
{0xFF, "call",		CODE_RM_SIZE16_32_64,	CODE_NONE,				CODE_OP_FIELD_02},
{0xFF, "call",		CODE_ADDRESS_OFF,		CODE_NONE,				CODE_OP_FIELD_03},
{0xFF, "jmp",		CODE_RM_SIZE16_32_64,	CODE_NONE,				CODE_OP_FIELD_04},
{0xFF, "jmp",		CODE_ADDRESS_OFF,		CODE_NONE,				CODE_OP_FIELD_05},
{0xFF, "push",		CODE_RM_SIZE16_32_64,	CODE_NONE,				CODE_OP_FIELD_06},

/* two-byte opcode */
{0x00, "sldt",		CODE_R_SIZE16_32_64,	CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_00},
{0x00, "sldt",		CODE_M_SIZE16,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_00},
{0x00, "str",		CODE_R_SIZE16_32_64,	CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_01},
{0x00, "str",		CODE_M_SIZE16,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_01},
{0x00, "lldt",		CODE_R_SIZE16_32_64,	CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_02},
{0x00, "lldt",		CODE_M_SIZE16,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_02},
{0x00, "ltr",		CODE_RM_SIZE16,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_03},
{0x00, "verr",		CODE_RM_SIZE16,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_04},
{0x00, "verw",		CODE_RM_SIZE16,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_05},
{0x01, "sdgt",		CODE_M_ALL,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_00},
{0x01, "vmcall",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD | CODE_SECOND_OPCODE | 0xC1},
{0x01, "vmlaunch",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD | CODE_SECOND_OPCODE | 0xC2},
{0x01, "vmresume",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD | CODE_SECOND_OPCODE | 0xC3},
{0x01, "vmxoff",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD | CODE_SECOND_OPCODE | 0xC4},
{0x01, "sidt",		CODE_M_ALL,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_01},
{0x01, "monitor",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_01 | CODE_SECOND_OPCODE | 0xC8},
{0x01, "mwait",		CODE_NONE,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_01 | CODE_SECOND_OPCODE | 0xC9},
{0x01, "lgdt",		CODE_M_ALL,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_02},
{0x01, "xgetbv",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_02 | CODE_SECOND_OPCODE | 0xD0},
{0x01, "xsetbv",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_02 | CODE_SECOND_OPCODE | 0xD1},
{0x01, "lidt",		CODE_M_ALL,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_03},
{0x01, "smsm",		CODE_M_ALL,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_04},
{0x01, "smsm",		CODE_R_SIZE16_32_64,	CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_04},
{0x01, "lmsm",		CODE_RM_SIZE16,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_06},
{0x01, "invlpg",	CODE_M_ALL,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_07},
{0x01, "swapgs",	CODE_M_ALL,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_07 | CODE_SECOND_OPCODE | 0xF8},
{0x01, "rdtscp",	CODE_M_ALL,				CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_07 | CODE_SECOND_OPCODE | 0xF9},
{0x02, "lar",		CODE_R_SIZE16_32_64,	CODE_M_SIZE16,			CODE_PRE_TWO | CODE_REG_FIELD},
{0x02, "lar",		CODE_R_SIZE16_32_64,	CODE_R_SIZE16_32,		CODE_PRE_TWO | CODE_REG_FIELD},
{0x03, "lsl",		CODE_R_SIZE16_32_64,	CODE_M_SIZE16,			CODE_PRE_TWO | CODE_REG_FIELD},
{0x03, "lsl",		CODE_R_SIZE16_32_64,	CODE_R_SIZE16_32,		CODE_PRE_TWO | CODE_REG_FIELD},
{0x05, "syscall",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x06, "clts",		CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x07, "sysret",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x08, "invd",		CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x09, "wbinvd",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x0B, "ud2",		CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x0D, "nop",		CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO},
{0x10, "movups",	CODE_XMM,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x10, "movups",	CODE_XMM,				CODE_M128,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x10, "movss",		CODE_XMM,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_SP},
{0x10, "movss",		CODE_XMM,				CODE_M32,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_SP},
{0x10, "movupd",	CODE_XMM,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_OPERAND},
{0x10, "movupd",	CODE_XMM,				CODE_M128,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_OPERAND},
{0x10, "movsd",		CODE_XMM,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_DP},
{0x10, "movs",		CODE_XMM,				CODE_M64,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_DP},
{0x11, "movups",	CODE_XMM,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x11, "movups",	CODE_M128,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x11, "movss",		CODE_XMM,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_SP},
{0x11, "movss",		CODE_M32,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_SP},
{0x11, "movupd",	CODE_XMM,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_OPERAND},
{0x11, "movupd",	CODE_M128,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_OPERAND},
{0x11, "movsd",		CODE_XMM,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_DP},
{0x11, "movs",		CODE_M64,				CODE_XMM,				CODE_PRE_TWO | CODE_REG_FIELD | CODE_PRE_DP},
{0x18, "prefetchnta", CODE_M_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x18, "prefetcht0", CODE_M_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_01},
{0x18, "prefetcht1", CODE_M_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_02},
{0x18, "prefetcht2", CODE_M_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_03},
{0x18, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_04},
{0x18, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_05},
{0x18, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_06},
{0x18, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_07},
{0x19, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO},
{0x1A, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO},
{0x1B, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO},
{0x1C, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO},
{0x1D, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO},
{0x1E, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO},
{0x1F, "nop",		CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x1F, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_01},
{0x1F, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_02},
{0x1F, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_03},
{0x1F, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_04},
{0x1F, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_05},
{0x1F, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_06},
{0x1F, "hint_nop",	CODE_RM_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_07},
{0x20, "mov",		CODE_R_SIZE32_64,		CODE_REG_CTR,			CODE_PRE_TWO | CODE_REG_FIELD},
{0x21, "mov",		CODE_R_SIZE32_64,		CODE_REG_DBG,			CODE_PRE_TWO | CODE_REG_FIELD},
{0x22, "mov",		CODE_REG_CTR,			CODE_R_SIZE32_64,		CODE_PRE_TWO | CODE_REG_FIELD},
{0x23, "mov",		CODE_REG_DBG,			CODE_R_SIZE32_64,		CODE_PRE_TWO | CODE_REG_FIELD},
{0x30, "wrmsr",		CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x31, "rdtsc",		CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x32, "rdmsr",		CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x33, "rdpmc",		CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x34, "sysenter",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x35, "sysexit",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x37, "sysexit",	CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x40, "cmovo",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x41, "cmovno",	CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x42, "cmovc",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x43, "cmovnc",	CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x44, "cmovz",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x45, "cmovbe",	CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x46, "cmovna",	CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x47, "cmova",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x48, "cmovs",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x49, "cmovns",	CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x4A, "cmovp",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x4B, "cmovnp",	CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x4C, "cmovl",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x4D, "cmovge",	CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x4E, "cmovle",	CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x4F, "cmovg",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0x77, "emms",		CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0x78, "vmread",	CODE_RM_SIZE64,			CODE_R_SIZE64,			CODE_PRE_TWO | CODE_REG_FIELD},
{0x79, "vmwrite",	CODE_R_SIZE64,			CODE_RM_SIZE64,			CODE_PRE_TWO | CODE_REG_FIELD},
{0x80, "jo",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x81, "jno",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x82, "jc",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x83, "jnc",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x84, "jz",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x85, "jnz",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x86, "jna",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x87, "ja",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x88, "js",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x89, "jns",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x8A, "jp",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x8B, "jnp",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x8C, "jl",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x8D, "jge",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x8E, "jle",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x8F, "jg",		CODE_REL_SIZE16_32,		CODE_NONE,				CODE_PRE_TWO | CODE_REG_FIELD},
{0x90, "seto",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x91, "setno",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x92, "setc",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x93, "setnc",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x94, "setz",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x95, "setnz",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x96, "setna",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x97, "seta",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x98, "sets",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x99, "setns",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x9A, "setp",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x9B, "setnp",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x9C, "setl",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x9D, "setge",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x9E, "setle",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0x9F, "setg",		CODE_RM_SIZE8,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD},
{0xA0, "push",		CODE_R_FS,				CODE_NONE,				CODE_PRE_TWO},
{0xA1, "pop	",		CODE_R_FS,				CODE_NONE,				CODE_PRE_TWO},
{0xA2, "cpuid",		CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0xA3, "bt",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0xA8, "push",		CODE_R_GS,				CODE_NONE,				CODE_PRE_TWO},
{0xA9, "pop	",		CODE_R_GS,				CODE_NONE,				CODE_PRE_TWO},
{0xAA, "rsm",		CODE_NONE,				CODE_NONE,				CODE_PRE_TWO},
{0xAB, "bts",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0xAF, "imul",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0xB0, "cmpxchg",	CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_PRE_TWO | CODE_REG_FIELD},
{0xB1, "cmpxchg",	CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0xB6, "movzx",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE8,			CODE_PRE_TWO | CODE_REG_FIELD},
{0xB7, "movzx",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16,			CODE_PRE_TWO | CODE_REG_FIELD},
{0xB8, "popcnt",	CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_PRE_SP | CODE_REG_FIELD},
/* UD */
{0xBA, "bt",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_PRE_TWO | CODE_OP_FIELD_04},
{0xBA, "bts",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_PRE_TWO | CODE_OP_FIELD_05},
{0xBA, "btr",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_PRE_TWO | CODE_OP_FIELD_06},
{0xBA, "btc",		CODE_RM_SIZE16_32_64,	CODE_I_SIZE8,			CODE_PRE_TWO | CODE_OP_FIELD_07},
{0xBB, "btc",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0xBC, "bsf",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0xBD, "bsr",		CODE_R_SIZE16_32_64,	CODE_RM_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
{0xBE, "movsx",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_PRE_TWO | CODE_REG_FIELD},
{0xBF, "movsx",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16,			CODE_PRE_TWO | CODE_REG_FIELD},
{0xC0, "xadd",		CODE_RM_SIZE8,			CODE_R_SIZE8,			CODE_PRE_TWO | CODE_REG_FIELD},
{0xC1, "xadd",		CODE_RM_SIZE16_32_64,	CODE_R_SIZE16_32_64,	CODE_PRE_TWO | CODE_REG_FIELD},
/* skip xmm related machine code */
{0xC7, "vmptrld",	CODE_M_SIZE64,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_06},
{0xC7, "vmclear",	CODE_M_SIZE64,			CODE_NONE,				CODE_PRE_TWO | CODE_PRE_OPERAND | CODE_OP_FIELD_06},
{0xC7, "vmxon",		CODE_M_SIZE64,			CODE_NONE,				CODE_PRE_TWO | CODE_PRE_SP | CODE_OP_FIELD_06},
{0xC7, "vmptrst",	CODE_M_SIZE64,			CODE_NONE,				CODE_PRE_TWO | CODE_OP_FIELD_07},
{0xC8, "bswap",		CODE_R_SIZE16_32_64,	CODE_NONE,				CODE_PRE_TWO | CODE_SECOND_OPCODE},
/* skip xmm related machine code */

