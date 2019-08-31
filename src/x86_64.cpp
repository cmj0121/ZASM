/* Copyright (C) 2017-2019 cmj. All right reserved. */

#include <sstream>
#include <iomanip>
#include <algorithm>

#include "zasm.h"


#define LOG_HEX_DUMP		LOG_WARN
#define LOG_DEBUG_X86_EMIT	LOG_WARN
#define MAX_X86_INSTNAME_LEN	16
#define MAX_X86_INSTRUCTION_LEN	16

#define HEX_PRETTY_PRINT 		std::setfill('0') << std::setw(2) << std::hex << std::uppercase
#define HEX_DUMP(msg,  nr)																	\
	do {																					\
		std::stringstream ss;																\
		if (this->_len_) {																	\
			ss << HEX_PRETTY_PRINT << (int)(this->_code_[this->_len_ - nr]);				\
			for (size_t i = 1; i < nr && i < this->_len_; ++i) {							\
				ss << " "<< HEX_PRETTY_PRINT << (int)(this->_code_[this->_len_ - nr + i]);	\
			}																				\
		}																					\
		_D(LOG_HEX_DUMP, "%-24s - %s", msg, ss.str().c_str());								\
	} while(0)

/* x86 related inner structure
 *
 * 0    1    2    4        7       8
 * +----+----+----+--------+-------+
 * | BS | EX | SZ | PREFIX | CONST |
 * +----+----+----+--------+-------+
 */
#define CODE_SZ_OFF			16
#define CODE_PREFIX_OFF		32
#define CODE_CONST_OFF		56
typedef enum {
	/* basic type of operand */
	CODE_NONE			= 0x00,
	CODE_REGISTER		= 0x01,
	CODE_MEMORY			= 0x02,
	CODE_IMMEDIATE		= 0x04,
	CODE_RELATED		= 0x08,

	/* extension */
	CODE_REG_SEG		= CODE_REGISTER | 0x10,
	CODE_REG_CTR		= CODE_REGISTER | 0x20,
	CODE_REG_DBG		= CODE_REGISTER | 0x30,
	CODE_REG_ST			= CODE_REGISTER | 0x40,
	CODE_R_ST_M32_REAL	= CODE_REGISTER | 0x50,
	CODE_M_SIZE14_28	= CODE_REGISTER | 0x60,

	CODE_CONSTANT		= CODE_IMMEDIATE | 0x10,
	CODE_ADDRESS_OFF	= CODE_IMMEDIATE | 0x20,	/* pre16:16 / ptr16:32 or m16:16 / m16:32 */
	CODE_REG_SPECIFIED	= CODE_REGISTER  | CODE_CONSTANT,
	CODE_SEG_OFF		= CODE_REGISTER  | CODE_ADDRESS_OFF,
	CODE_BS_MASK		= 0xFF,

	/* size related decorator */
	CODE_SIZE8			= (0x01 << CODE_SZ_OFF),
	CODE_SIZE16			= (0x02 << CODE_SZ_OFF),
	CODE_SIZE32			= (0x04 << CODE_SZ_OFF),
	CODE_SIZE64			= (0x08 << CODE_SZ_OFF),
	CODE_SIZE128		= (0x10 << CODE_SZ_OFF),	/* xmm */
	CODE_SIZE256		= (0x20 << CODE_SZ_OFF),
	CODE_SIZE512		= (0x40 << CODE_SZ_OFF),
	CODE_SIZE_MASK		= (0xFF << CODE_SZ_OFF),

	/* specify prefix */
	CODE_X86_ONLY		= ((long)0x00001 << CODE_PREFIX_OFF),
	CODE_PRE_TWO		= ((long)0x00002 << CODE_PREFIX_OFF),	/* 0x0F - two-byte opcode         */
	CODE_PRE_ES			= ((long)0x00004 << CODE_PREFIX_OFF),	/* 0x26 - segment ES              */
	CODE_PRE_CS			= ((long)0x00008 << CODE_PREFIX_OFF),	/* 0x2E - segment CS              */
	CODE_PRE_SS			= ((long)0x00010 << CODE_PREFIX_OFF),	/* 0x36 - segment SS              */
	CODE_PRE_DS			= ((long)0x00020 << CODE_PREFIX_OFF),	/* 0x3E - segment DS              */
	CODE_PRE_FS			= ((long)0x00040 << CODE_PREFIX_OFF),	/* 0x64 - segment FS              */
	CODE_PRE_GS			= ((long)0x00080 << CODE_PREFIX_OFF),	/* 0x65 - segment GS              */
	CODE_PRE_OPERAND	= ((long)0x00100 << CODE_PREFIX_OFF),	/* 0x66 - operand override        */
	CODE_PRE_ADDRESS	= ((long)0x00200 << CODE_PREFIX_OFF),	/* 0x67 - address override        */
	CODE_PRE_WAIT		= ((long)0x00400 << CODE_PREFIX_OFF),	/* 0x9B - WAIT                    */
	CODE_PRE_LOCK		= ((long)0x00800 << CODE_PREFIX_OFF),	/* 0xF0 - LOCK                    */
	CODE_PRE_DP			= ((long)0x01000 << CODE_PREFIX_OFF),	/* 0xF2 - scalar double-precision */
	CODE_PRE_SP			= ((long)0x02000 << CODE_PREFIX_OFF),	/* 0xF3 - scalar single-precision */
	CODE_OP_FIELD		= ((long)0x04000 << CODE_PREFIX_OFF),	/* opcode field   */
	CODE_REG_FIELD		= ((long)0x08000 << CODE_PREFIX_OFF),	/* register field */
	CODE_SECOND_OPCODE	= ((long)0x10000 << CODE_PREFIX_OFF),	/* second opcode  */
	CODE_FORCE_64BIT	= ((long)0x20000 << CODE_PREFIX_OFF),
	CODE_PREFIX_MASK	= ((long)0xFFFFF << CODE_PREFIX_OFF),

	/* syntax sugar */
	CODE_XMM			= CODE_REGISTER | CODE_SIZE128,
	CODE_M128			= CODE_MEMORY   | CODE_SIZE128,
	CODE_M32			= CODE_MEMORY   | CODE_SIZE32,
	CODE_M64			= CODE_MEMORY   | CODE_SIZE64,
	CODE_M_ALL			= CODE_MEMORY   | CODE_SIZE_MASK,

	CODE_OP_FIELD_00	= CODE_OP_FIELD,
	CODE_OP_FIELD_01	= CODE_OP_FIELD | ((long)0x01 << CODE_CONST_OFF),
	CODE_OP_FIELD_02	= CODE_OP_FIELD | ((long)0x02 << CODE_CONST_OFF),
	CODE_OP_FIELD_03	= CODE_OP_FIELD | ((long)0x03 << CODE_CONST_OFF),
	CODE_OP_FIELD_04	= CODE_OP_FIELD | ((long)0x04 << CODE_CONST_OFF),
	CODE_OP_FIELD_05	= CODE_OP_FIELD | ((long)0x05 << CODE_CONST_OFF),
	CODE_OP_FIELD_06	= CODE_OP_FIELD | ((long)0x06 << CODE_CONST_OFF),
	CODE_OP_FIELD_07	= CODE_OP_FIELD | ((long)0x07 << CODE_CONST_OFF),

	CODE_CONSTANT_1		= CODE_CONSTANT | ((long)0x01 << CODE_CONST_OFF),
	CODE_CONSTANT_3		= CODE_CONSTANT | ((long)0x03 << CODE_CONST_OFF),

	CODE_SIZE16_32			= CODE_SIZE16 | CODE_SIZE32,
	CODE_SIZE16_64			= CODE_SIZE16 | CODE_SIZE64,
	CODE_SIZE32_64			= CODE_SIZE32 | CODE_SIZE64,
	CODE_SIZE16_32_64		= CODE_SIZE16 | CODE_SIZE32 | CODE_SIZE64,

	CODE_R_SIZE8			= CODE_SIZE8 | CODE_REGISTER,
	CODE_M_SIZE8			= CODE_SIZE8 | CODE_MEMORY,
	CODE_I_SIZE8			= CODE_SIZE8 | CODE_IMMEDIATE,
	CODE_REL_SIZE8			= CODE_SIZE8 | CODE_RELATED,
	CODE_SEG_OFF_SIZE8		= CODE_SIZE8 | CODE_SEG_OFF,

	CODE_R_SIZE16			= CODE_SIZE16 | CODE_REGISTER,
	CODE_M_SIZE16			= CODE_SIZE16 | CODE_MEMORY,
	CODE_I_SIZE16			= CODE_SIZE16 | CODE_IMMEDIATE,
	CODE_REL_SIZE16			= CODE_SIZE16 | CODE_RELATED,

	CODE_R_SIZE32			= CODE_SIZE32 | CODE_REGISTER,
	CODE_M_SIZE32			= CODE_SIZE32 | CODE_MEMORY,
	CODE_I_SIZE32			= CODE_SIZE32 | CODE_IMMEDIATE,
	CODE_REL_SIZE32			= CODE_SIZE32 | CODE_RELATED,

	CODE_R_SIZE64			= CODE_SIZE64 | CODE_REGISTER,
	CODE_M_SIZE64			= CODE_SIZE64 | CODE_MEMORY,
	CODE_I_SIZE64			= CODE_SIZE64 | CODE_IMMEDIATE,
	CODE_REL_SIZE64			= CODE_SIZE64 | CODE_RELATED,

	CODE_R_SIZE16_32		= CODE_SIZE16_32 | CODE_REGISTER,
	CODE_M_SIZE16_32		= CODE_SIZE16_32 | CODE_MEMORY,
	CODE_I_SIZE16_32		= CODE_SIZE16_32 | CODE_IMMEDIATE,
	CODE_REL_SIZE16_32		= CODE_SIZE16_32 | CODE_RELATED,

	CODE_R_SIZE16_64		= CODE_SIZE16_64 | CODE_REGISTER,
	CODE_M_SIZE16_64		= CODE_SIZE16_64 | CODE_MEMORY,
	CODE_I_SIZE16_64		= CODE_SIZE16_64 | CODE_IMMEDIATE,
	CODE_R_SIZE32_64		= CODE_SIZE32_64 | CODE_REGISTER,
	CODE_M_SIZE32_64		= CODE_SIZE32_64 | CODE_MEMORY,

	CODE_R_SIZE16_32_64			= CODE_SIZE16_32_64 | CODE_REGISTER,
	CODE_M_SIZE16_32_64			= CODE_SIZE16_32_64 | CODE_MEMORY,
	CODE_I_SIZE16_32_64			= CODE_SIZE16_32_64 | CODE_IMMEDIATE,
	CODE_SEG_OFF_SIZE16_32_64	= CODE_SIZE16_32_64 | CODE_SEG_OFF,

	CODE_RM_SIZE8			= CODE_R_SIZE8 | CODE_M_SIZE8,
	CODE_RM_SIZE16			= CODE_R_SIZE16 | CODE_M_SIZE16,
	CODE_RM_SIZE64			= CODE_R_SIZE64 | CODE_M_SIZE64,
	CODE_RM_SIZE16_32		= CODE_R_SIZE16_32 | CODE_M_SIZE16_32,
	CODE_RM_SIZE16_64		= CODE_R_SIZE16_64 | CODE_M_SIZE16_64,
	CODE_RM_SIZE16_32_64	= CODE_R_SIZE16_32_64 | CODE_M_SIZE16_32_64,

	CODE_RAX				= CODE_REG_SPECIFIED | CODE_SIZE64 | ((long)0x00 << CODE_CONST_OFF),
	CODE_RCX				= CODE_REG_SPECIFIED | CODE_SIZE64 | ((long)0x01 << CODE_CONST_OFF),
	CODE_RDX				= CODE_REG_SPECIFIED | CODE_SIZE64 | ((long)0x02 << CODE_CONST_OFF),
	CODE_RBX				= CODE_REG_SPECIFIED | CODE_SIZE64 | ((long)0x03 << CODE_CONST_OFF),
	CODE_EAX				= CODE_REG_SPECIFIED | CODE_SIZE32 | ((long)0x00 << CODE_CONST_OFF),
	CODE_ECX				= CODE_REG_SPECIFIED | CODE_SIZE32 | ((long)0x01 << CODE_CONST_OFF),
	CODE_EDX				= CODE_REG_SPECIFIED | CODE_SIZE32 | ((long)0x02 << CODE_CONST_OFF),
	CODE_EBX				= CODE_REG_SPECIFIED | CODE_SIZE32 | ((long)0x03 << CODE_CONST_OFF),
	CODE_AX					= CODE_REG_SPECIFIED | CODE_SIZE16 | ((long)0x00 << CODE_CONST_OFF),
	CODE_CX					= CODE_REG_SPECIFIED | CODE_SIZE16 | ((long)0x01 << CODE_CONST_OFF),
	CODE_DX					= CODE_REG_SPECIFIED | CODE_SIZE16 | ((long)0x02 << CODE_CONST_OFF),
	CODE_BX					= CODE_REG_SPECIFIED | CODE_SIZE16 | ((long)0x03 << CODE_CONST_OFF),
	CODE_AL					= CODE_REG_SPECIFIED | CODE_SIZE8  | ((long)0x00 << CODE_CONST_OFF),
	CODE_CL					= CODE_REG_SPECIFIED | CODE_SIZE8  | ((long)0x01 << CODE_CONST_OFF),
	CODE_DL					= CODE_REG_SPECIFIED | CODE_SIZE8  | ((long)0x02 << CODE_CONST_OFF),
	CODE_BL					= CODE_REG_SPECIFIED | CODE_SIZE8  | ((long)0x03 << CODE_CONST_OFF),
	CODE_AH					= CODE_REG_SPECIFIED | ((long)0x00 << CODE_CONST_OFF),
	CODE_CH					= CODE_REG_SPECIFIED | ((long)0x01 << CODE_CONST_OFF),
	CODE_DH					= CODE_REG_SPECIFIED | ((long)0x02 << CODE_CONST_OFF),
	CODE_BH					= CODE_REG_SPECIFIED | ((long)0x03 << CODE_CONST_OFF),


	CODE_R_CS				= CODE_REG_SEG | ((long)0x01 << CODE_CONST_OFF),
	CODE_R_DS				= CODE_REG_SEG | ((long)0x02 << CODE_CONST_OFF),
	CODE_R_SS				= CODE_REG_SEG | ((long)0x03 << CODE_CONST_OFF),
	CODE_R_ES				= CODE_REG_SEG | ((long)0x04 << CODE_CONST_OFF),
	CODE_R_FS				= CODE_REG_SEG | ((long)0x05 << CODE_CONST_OFF),
	CODE_R_GS				= CODE_REG_SEG | ((long)0x06 << CODE_CONST_OFF),
} X86_TYPE;

typedef struct {
	unsigned char opcode;
	char command[MAX_X86_INSTNAME_LEN];
	long dst;
	long src;
	long flags;
} X86_INSTRUCTIONS;

static X86_INSTRUCTIONS insts[] = {
	#include "x86_64_opcode.h"
};

#include "x86_64_operand.h"

#define OPERAND_FMT_SIZE	22

void ZasmCode::emit(std::string opcode, std::vector<ZasmToken> operands, ZasmCodeArch arch, off_t vaddr) {
	std::stringstream ss;
	Operand dst, src;

	ss << std::setw(8) << opcode;
	switch (operands.size()) {
		case 3:
			/* operand with size specified */
			if (operands[1].type == ZASMT_COLON) {
				dst = Operand(operands[0], operands[2]);
				ss << std::setw(OPERAND_FMT_SIZE) << (operands[0].raw + operands[1].raw + operands[2].raw);
				break;
			}

			if (operands[0].type == ZASMT_MEM_SIZE) {
				dst = Operand(operands[1]);
				src = Operand(operands[2]);
				dst.set_size(operands[0]);

				ss << std::setw(OPERAND_FMT_SIZE) << (operands[0].raw + " " + operands[1].raw);
				ss << std::setw(OPERAND_FMT_SIZE) << operands[2].raw;
			} else if (operands[1].type == ZASMT_MEM_SIZE) {
				dst = Operand(operands[0]);
				src = Operand(operands[2]);
				src.set_size(operands[1]);

				ss << std::setw(OPERAND_FMT_SIZE) << operands[0].raw;
				ss << std::setw(OPERAND_FMT_SIZE) << (operands[1].raw + " " + operands[2].raw);
			} else {
				_D(LOG_CRIT, "Not support code type %X %X %X", operands[0].type, operands[1].type, operands[2].type);
				throw Exception(ERR_NOT_IMPLEMENTED, "Not support code type");
			}
			break;
		case 2:
			if (operands[0].type == ZASMT_MEM_SIZE) {
				dst = Operand(operands[1]);
				dst.set_size(operands[0]);

				ss << std::setw(OPERAND_FMT_SIZE) << (operands[0].raw + " " + operands[1].raw);
				break;
			}
			src = Operand(operands[1]);
			dst = Operand(operands[0]);
			ss << std::setw(OPERAND_FMT_SIZE) << ((ZASMT_TOKEN == operands[0].type ? "&" : "") + operands[0].raw);
			ss << std::setw(OPERAND_FMT_SIZE) << ((ZASMT_TOKEN == operands[0].type ? "&" : "") + operands[1].raw);
			break;
		case 1:
			dst = Operand(operands[0]);
			ss << std::setw(OPERAND_FMT_SIZE) << ((ZASMT_TOKEN == operands[0].type ? "&" : "") + operands[0].raw);
		case 0:
			break;
		default:
			_D(LOG_CRIT, "Syntax error - too many operands %zu", operands.size());
			throw Exception(ERR_SYNTAX_ERROR, "Syntax error - too many operands");
			break;
	}
	this->_fmt_   = ss.str();
	this->_vaddr_ = vaddr;

	for (auto inst : insts) {
		if (opcode != inst.command) {
			/* not match the opcode */
			continue;
		} else if (! dst.match(inst.dst) || ! src.match(inst.src)) {
			/* operand not match */
			continue;
		} else if (CODE_X86_ONLY == (inst.flags & CODE_X86_ONLY) && arch != ARCH_X86_REAL_MODE) {
			/* platform not match */
			_D(LOG_CRIT, "%lX %X", inst.flags, arch);
			continue;
		}

		this->_code_ = new (unsigned char )(MAX_X86_INSTRUCTION_LEN);
		_D(LOG_DEBUG_X86_EMIT, "start emit the machine code 0x%02X (%d)...", inst.opcode, arch);

		/* detect the memory register and memory register */
		unsigned char rel = ARCH_X86_REAL_MODE == arch ? 0x06 : 0x05;
		Operand op_reg = (ZASMT_MEM == src._type_ || ZASMT_REF == src._type_ ? dst : src),
				op_mem = (ZASMT_MEM == src._type_ || ZASMT_REF == src._type_ ? src : dst);

		{	/* legacy prefix*/
			{	/* legacy prefix - group 1 (LOCK/REPNE/REP)          */
				if (0 != (inst.flags & CODE_PRE_WAIT)) this->_code_[this->_len_ ++] = 0x9B;
				if (0 != (inst.flags & CODE_PRE_LOCK)) this->_code_[this->_len_ ++] = 0xF0;
				if (0 != (inst.flags & CODE_PRE_DP))   this->_code_[this->_len_ ++] = 0xF2;
				if (0 != (inst.flags & CODE_PRE_SP))   this->_code_[this->_len_ ++] = 0xF3;
			}
			{	/* legacy prefix - group 2 (segment register/branch) */
				if ((dst._reg_seg_ && CODE_REG_SEG != (inst.dst & CODE_REG_SEG)) ||
					(src._reg_seg_ && CODE_REG_SEG != (inst.src & CODE_REG_SEG)) || (dst._mem_seg_ || src._mem_seg_)) {
					int pos = dst._mem_seg_ ? (dst._mem_seg_->_reg_pos_) :
								(src._mem_seg_ ? src._mem_seg_->_reg_pos_ : (dst._reg_pos_ | src._reg_pos_));

					switch (pos) {
						case 0:
						case 1:
						case 2:
						case 3:
							this->_code_[this->_len_ ++] = 0x26 + 0x08 * pos;
							HEX_DUMP("segment prefix", 1);
							break;
						case 4:
							this->_code_[this->_len_ ++] = 0x64;
							HEX_DUMP("FS prefix", 1);
							break;
						case 5:
							this->_code_[this->_len_ ++] = 0x65;
							HEX_DUMP("GS prefix", 1);
							break;
						default:
							_D(LOG_CRIT, "Not Implemented #%d", pos);
							throw Exception(ERR_UNKNOWN, "Not Implemented");
							break;
					}
				}
			}
			{	/* legacy prefix - group 3 (operand-size override)   */
				if (0 != (inst.flags & CODE_PRE_OPERAND)) {
					this->_code_[this->_len_ ++] = 0x66;
					HEX_DUMP("operand-size override", 1);
				} else if (ARCH_X86_REAL_MODE == arch) {
					/* nop */
				} else if (dst._size_ == CODE_OPERAND_SIZE_16 || src._size_ == CODE_OPERAND_SIZE_16) {
					this->_code_[this->_len_ ++] = 0x66;
					HEX_DUMP("operand-size override", 1);
				}
			}
			{	/* legacy prefix - group 4 (address-size override)   */
			}

			if (ARCH_X86_REAL_MODE != arch) {	/* 64-bit operand size prefix */
				/*
				 * 7             4               0
				 * +-------------+---+---+---+---+
				 * | 0  1  0  0  | W | R | X | B |
				 * +-------------+---+---+---+---+
				 * REX.B	Extension to the Mod R/M.rm field or the SIB.base field
				 * REX.X	Extension to the SIB.index field
				 * REX.R	Extension to the Mod R/M.reg field
				 * REX.W	64-bit operand size is used.
				 */
				if (0 != (CODE_OPERAND_SIZE_64 & (dst._size_ | src._size_)) || dst._reg_ext_ || src._reg_ext_) {
					int REX_B = 0, REX_X = 0, REX_R = 0, REX_W = 0;
					unsigned char ext = 0x40;

					if (op_mem._reg_ext_)										{ REX_B = 1; }
					if (op_mem._mem_index_ && op_mem._mem_index_->_reg_ext_)	{ REX_X = 1; }
					if (op_reg._reg_ext_)										{ REX_R = 1; }

					if (CODE_OPERAND_SIZE_64 == (CODE_OPERAND_SIZE_64 & (dst._size_ | src._size_))) {
						if (0xB8 == inst.opcode && CODE_OPERAND_SIZE_64 != src._size_) {
							/* bypass the 64-bit extension */
							;;
						} else {
							REX_W = 1;
						}
					}

					ext |= (REX_W << 3) | (REX_R << 2) | (REX_X << 1) | REX_B;

					this->_code_[this->_len_ ++] = ext;
					HEX_DUMP("64-bit extension", 1);
				}
			}
		}

		{	/* opcode */
			if (0 != (inst.flags & CODE_PRE_TWO)) {
				this->_code_[ this->_len_++ ] = 0x0F;
				HEX_DUMP("two-bytes prefix", 1);
			}

			this->_code_[ this->_len_++ ] = inst.opcode;

			if (0 != (CODE_SECOND_OPCODE & inst.flags)) {
				this->_code_[this->_len_ - 1] |= dst._reg_pos_;
				if (dst._reg_highbyte_) this->_code_[ this->_len_ - 1 ] |= 0x04;
			}
			HEX_DUMP("opcode", 1);
		}

		{	/* mod R/M */
			/* 7     5         2        0
			 * |-----|---------|--------|
			 * | mod |   reg   |   rm   |
			 * |-----|---------|--------|
			 *
			 * mod -
			 *        Usually is b11 which register direct addressing, otherwise
			 *        register-indirect addressing mode used.
			 *
			 *			00		[r + r]							[r + r]
			 *			01		[r + r + displacement 8]		[r + r + displacement 8]
			 *			10		[r + r + displacement 16]		[r + r + displacement 32]
			 *			11		r / m							r / m
			 * reg -
			 *        1) 3bit opcode extension
			 *        2) 3bit register reference
			 * rm  -
			 *        Direct or indirect register operand, optionally with a displacement
			 */
			int mod = 0, reg = 0, rm = 0;

			/* process the MOD */
			switch (op_mem._type_) {
				case ZASMT_UNKONWN:
				case ZASMT_REG:
				case ZASMT_IMM:
					mod = 0x03;	/* r / r */
					break;
				case ZASMT_REF:
					mod = 0x00;
					break;
				case ZASMT_MEM:
				case ZASMT_ADDR_OFF:
					mod = 0x00;
					if (op_mem._mem_based_) {
						if (ABS(op_mem._mem_offset_))			mod = 0x01;
						if (ABS(op_mem._mem_offset_) & ~0x7F)	mod = 0x02;
					}
					break;
				default:
					_D(LOG_CRIT, "Not Implemented #%d type", op_mem._type_);
					throw Exception(ERR_UNKNOWN, "Not Implemented");
					break;
			}


			if (op_mem._mem_scalar_) {
				rm = 0x04;
			}

			if (CODE_REG_FIELD == (CODE_REG_FIELD & inst.flags)) {
				switch (op_mem._type_) {
					case ZASMT_REG:
						rm  = op_reg._reg_pos_ | (op_reg._reg_highbyte_ ? 0x04 : 0x00);
						reg = op_mem._reg_pos_ | (op_mem._reg_highbyte_ ? 0x04 : 0x00);

						if (op_reg._type_ == ZASMT_IMM) {
							/* NOTE - Special case or 0x69/0x6B with duplicate reg field */
							rm = reg;
						}
						break;
					default:
						reg = op_reg._reg_pos_ | (op_reg._reg_highbyte_ ? 0x04 : 0x00);
						rm  = op_mem._type_ == ZASMT_REF ? rel : op_mem._reg_pos_;
						rm  = rm | (op_mem._reg_highbyte_ ? 0x04 : 0x00);
						break;
				}

				this->_code_[this->_len_ ++] = (mod & 0x03) << 6 | (reg & 0x07) << 3 | (rm & 0x07);
				HEX_DUMP("Mod R/M", 1);
			} else if (CODE_OP_FIELD == (CODE_OP_FIELD & inst.flags)) {
				reg = (inst.flags >> CODE_CONST_OFF) & 0x0F;

				switch (op_mem._type_) {
					case ZASMT_UNKONWN:
					case ZASMT_IMM:
					case ZASMT_REG:
						rm |= op_mem._reg_pos_;
						break;
					case ZASMT_MEM:
					case ZASMT_REF:
						if (NULL == op_mem._mem_based_ && NULL == op_mem._mem_index_) rm |= rel;
						rm |= (op_mem._mem_based_ ? op_mem._mem_based_->_reg_pos_ : 0x00);
						break;
					default:
						_D(LOG_CRIT, "Not support operand type %d", op_mem._type_);
						throw Exception(ERR_NOT_IMPLEMENTED, "not implemented");
						break;
				}

				this->_code_[this->_len_ ++] = (mod & 0x03) << 6 | (reg & 0x07) << 3 | (rm & 0x07);
				HEX_DUMP("Mod R/M", 1);
			}


			if (ZASMT_MEM == op_mem._type_ && (op_mem._mem_index_ || op_mem._mem_scalar_)) {	/* SIB */
				/* 7       5         2        0
				 * |-------|---------|--------|
				 * | scale |  index  |  base  |
				 * |-------|---------|--------|
				 *
				 * scale - size of the SIB.index.
				 * index - the index register to use.
				 * base  - the base register to use.
				 */
				int scale = 0, index = 0, base = 0;
				unsigned char sib = 0x00;

				scale = LOG2(op_mem._mem_scalar_);
				index = op_mem._mem_index_->_reg_pos_;
				base  = op_mem._reg_pos_;

				sib = (scale & 0x03) << 6 | (index & 0x07) << 3 | (base & 0x07);
				this->_code_[this->_len_ ++] = sib;
				HEX_DUMP("SIB", 1);
			} else if (op_mem._mem_based_ && op_mem._mem_based_->_reg_pos_ == 0x04) {
				this->_code_[this->_len_ ++] = 0x24;
				HEX_DUMP("SIB", 1);
			}
		}

		{	/* displacement */
			Operand tgt = ZASMT_MEM == dst._type_ ? dst : src;

			if (ZASMT_MEM == tgt._type_) {
				if (tgt._mem_scalar_ || !tgt._mem_based_ || tgt._mem_offset_) {
					switch (arch) {
						case ARCH_X86_REAL_MODE:
							if (ABS(tgt._mem_offset_) < (((off_t)1 << 7) - 1)) {
								this->set_immediate(tgt._mem_offset_, CODE_OPERAND_SIZE_16);
								HEX_DUMP("displacement", 2);
								break;
							}
						case ARCH_X86_LONG_MODE:
							if (ABS(tgt._mem_offset_) < (((off_t)1 << 7) - 1)) {
								this->set_immediate(tgt._mem_offset_, CODE_OPERAND_SIZE_8);
								HEX_DUMP("displacement", 1);
								break;
							}
						default:
							this->set_immediate(tgt._mem_offset_, CODE_OPERAND_SIZE_32);
							HEX_DUMP("displacement", 4);
							break;
					}
				}
			}
		}

		{	/* immediate */
			if (dst._type_ == ZASMT_REF) {
				CodeSize size = CODE_OPERAND_SIZE_UNKONWN;

				switch (inst.dst & CODE_SIZE_MASK) {
					case CODE_SIZE8:
						size = CODE_OPERAND_SIZE_8;
						break;
					case CODE_SIZE16:
						size = CODE_OPERAND_SIZE_16;
						break;
					default:
						size = ARCH_X86_REAL_MODE == arch ? CODE_OPERAND_SIZE_16 : CODE_OPERAND_SIZE_32;
						break;
				}
				this->set_immediate(0x00, size);
				HEX_DUMP("immediate", size);

				this->_undefined_	= true;
				this->_absaddr_		= CODE_MEMORY == (CODE_MEMORY & inst.dst);
				this->_symb_		= dst._token_.raw;
			} else if (src._type_ == ZASMT_REF) {
				CodeSize size = ARCH_X86_REAL_MODE == arch ? CODE_OPERAND_SIZE_16 : CODE_OPERAND_SIZE_32;
				this->set_immediate(0x00, size);
				HEX_DUMP("immediate", size);

				this->_undefined_	= true;
				this->_absaddr_		= CODE_MEMORY == (CODE_MEMORY & inst.src) && !("lea" == opcode && arch == ARCH_X86_LONG_MODE);
				this->_symb_		= src._token_.raw;
			} else if (dst._type_ == ZASMT_ADDR_OFF) {
				off_t value = 0x00;

				this->_absaddr_ = true;
				switch (dst._rel_second->_type_) {
					case ZASMT_IMM:
						BUG(false == asInt(dst._rel_second->_token_.raw, value));
						if (value & ~0xFFFF) {
							_D(LOG_CRIT, "Not support related address %s", dst._token_.raw.c_str());
							throw Exception(ERR_SYNTAX_ERROR, "Not support related address `" + dst._token_.raw +"`");
						}

						this->set_immediate(value, CODE_OPERAND_SIZE_16);
						HEX_DUMP("immediate", CODE_OPERAND_SIZE_16);
						break;
					case ZASMT_REF:
						this->_undefined_ = true;
						this->_symb_      = dst._rel_second->_token_.raw;
						this->_addr_off_  = true;
						this->set_immediate(value, CODE_OPERAND_SIZE_16);
						HEX_DUMP("immediate", CODE_OPERAND_SIZE_16);
						break;
					default:
						_D(LOG_CRIT, "Not support related address %s", dst._token_.raw.c_str());
						throw Exception(ERR_SYNTAX_ERROR, "Not support related address `" + dst._token_.raw +"`");
						break;
				}

				switch (dst._rel_first->_type_) {
					case ZASMT_IMM:
						BUG(false == asInt(dst._rel_first->_token_.raw, value));
						if (value & ~0xFFFF) {
							_D(LOG_CRIT, "Not support related address %s", dst._token_.raw.c_str());
							throw Exception(ERR_SYNTAX_ERROR, "Not support related address `" + dst._token_.raw +"`");
						}

						this->set_immediate(value, CODE_OPERAND_SIZE_16);
						HEX_DUMP("immediate", CODE_OPERAND_SIZE_16);
						break;
					default:
						_D(LOG_CRIT, "Not support related address %s", dst._token_.raw.c_str());
						throw Exception(ERR_SYNTAX_ERROR, "Not support related address `" + dst._token_.raw +"`");
						break;
				}
			} else if (dst._type_ == ZASMT_IMM || src._type_ == ZASMT_IMM) {
				off_t value = 0x00;
				long flag = dst._type_ == ZASMT_IMM ? inst.dst : inst.src;
				CodeSize size = CODE_OPERAND_SIZE_UNKONWN;
				Operand imm = dst._type_ == ZASMT_IMM ? dst : src;


				this->_absaddr_ = 0 != (CODE_ADDRESS_OFF & flag);

				switch ( flag & CODE_SIZE_MASK) {
					case 0:
						if (CODE_CONSTANT == (flag & CODE_CONSTANT)) {
							break;
						} else if (CODE_RELATED == (flag & CODE_RELATED)) {
							size = CODE_OPERAND_SIZE_16;
							BUG(false == asInt(imm._token_.raw, value));
							break;
						}
						_D(LOG_CRIT, "should be constant immediate number");
						throw Exception(ERR_NOT_IMPLEMENTED, "should be constant immediate number");
					case CODE_SIZE8:
						size = CODE_OPERAND_SIZE_8;
						BUG(false == asInt(imm._token_.raw, value));
						break;
					case CODE_SIZE16:
						size = CODE_OPERAND_SIZE_16;
						BUG(false == asInt(imm._token_.raw, value));
						break;
					case CODE_SIZE16_32:
					case CODE_SIZE16_32_64:
						if (dst._size_ == CODE_OPERAND_SIZE_16) {
							size = CODE_OPERAND_SIZE_16;
							BUG(false == asInt(imm._token_.raw, value));
							break;
						}
					case CODE_SIZE32:
					case CODE_SIZE32_64:
					case CODE_SIZE64:
						BUG(false == asInt(imm._token_.raw, value));
						size = ARCH_X86_REAL_MODE == arch ? CODE_OPERAND_SIZE_16 : CODE_OPERAND_SIZE_32;
						break;
					case CODE_SIZE_MASK:
					default:
						if (flag == CODE_CONSTANT) {
							/* constant immediate and need NOT set immediate */
							break;
						}
						_D(LOG_CRIT, "Not Implemented code size - %llX", (long long)(inst.dst & CODE_SIZE_MASK));
						throw Exception(ERR_NOT_IMPLEMENTED, "Not Implemented code size");
						break;
				}

				if (size != CODE_OPERAND_SIZE_UNKONWN) {
					this->set_immediate(value, size);
					HEX_DUMP("immediate", size);
				}
			}
		}

		goto END;
	}

	throw Exception(ERR_SYNTAX_ERROR, "instruction `" + opcode + "` not found");
END:
	return ;
}

