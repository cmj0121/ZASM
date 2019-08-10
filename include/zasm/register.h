/* Copyright (C) 2017-2019 cmj. All right reserved. */
#pragma once

#define X86_REGISTER_64	"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi"
#define X86_REGISTER_32	"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"
#define X86_REGISTER_16	"ax" , "cx" , "dx" , "bx" , "sp" , "bp" , "si" , "di"
#define X86_REGISTER_8	"al" , "cl" , "dl" , "bl" , "spl", "bpl", "sil", "dil", \
						"ah" , "ch" , "dh" , "bh" , ""   , ""   , ""   ,  ""
#define X86_GENERAL_REG	X86_REGISTER_64, X86_REGISTER_32, X86_REGISTER_16, X86_REGISTER_8

#define X86_REGISTER_EXT	"r8" ,  "r9", "r10" , "r11" , "r12" , "r13" , "r14" , "r15" ,	\
							"r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d",	\
							"r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w",	\
							"r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b"

#define X86_REGISTER_SEG	"es", "cs", "ss", "ds", "fs", "gs", "", ""

#define CTRL_REGISTER		"cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7"
#define DEBUG_REGISTER		"dr0", "dr1", "dr2", "dr3", "dr4", "dr5", "dr6", "dr7"

#define REGISTERS	X86_GENERAL_REG, X86_REGISTER_EXT, X86_REGISTER_SEG, \
					CTRL_REGISTER, DEBUG_REGISTER

#define USED_REGISTER		"rcx", "rdx", "rbx", "r9", "r10", "r11"
#define X86_SYSCALL_REG		"rax", "rdi", "rsi", "rdx", "r10", "r8", "r9"
#define X86_SYSCALL_RET_REG	"rax"
