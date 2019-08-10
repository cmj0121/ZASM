/* Copyright (C) 2017-2019 cmj. All right reserved. */
#pragma once

/* decorator */
#define DECORATOR_REALMODE	"real"
#define DECORATOR_PROTMODE	"protected"
#define DECORATOR_VM_ADDR	"vaddr"

typedef enum {
	CODE_OPERAND_SIZE_UNKONWN	= 0x00,
	CODE_OPERAND_SIZE_8			= 0x01,
	CODE_OPERAND_SIZE_16		= 0x02,
	CODE_OPERAND_SIZE_32		= 0x04,
	CODE_OPERAND_SIZE_64		= 0x08,
} CodeSize;

typedef enum {
	ARCH_UNKNOWN = 0,		/* unknown CPU architecture */
	ARCH_X86_REAL_MODE,		/* x86 real mode            */
	ARCH_X86_PROT_MODE,		/* x86 protected mode       */
	ARCH_X86_LONG_MODE,		/* x86 long mode            */
} ZasmCodeArch;

/* pseudo machine code */
class ZasmCode {
	public:
		ZasmCode(void);
		/* create as pseudo machine code with only symbol */
		/* used for create memory buffer, may has symbol */
		ZasmCode(std::string symb);
		virtual ~ZasmCode(void);


		void set_ref(std::string ref);
		void set_data(std::string escaped_string);

		void set_vaddr(off_t vaddr)	{ this->_vaddr_ = vaddr; }
		void set_repeat(ZasmToken &from, ZasmToken &to);
		void set_immediate(off_t value, CodeSize size);
		void set_immediate(off_t value, off_t seclen, off_t curlen);
		void set_decorator(std::string key, std::string value);
		void emit(std::string opcode, std::vector<ZasmToken> operands, ZasmCodeArch arch, off_t vaddr);

		size_t length(void)					{ return this->_len_ * this->_repeat_; };
		std::string label(void)				{ return this->_label_;                };
		std::string symbol(void)			{ return this->_symb_;                 };

		operator int(void) const	{ return this->_undefined_; }
		friend std::fstream& operator << (std::fstream &st, const ZasmCode &code);	/* dump to file as machine code */
	private:
		unsigned char	*_code_;		/* machine code or memory buffer */
		std::string		_fmt_;			/* raw string that show the code */
		size_t			_len_;			/* length of machine code        */
		off_t			_vaddr_;		/* virtual address               */
		CodeSize		_imm_len_;		/* immediate length of code      */
		bool			_undefined_;	/* exist undefined symbol        */
		bool			_absaddr_;		/* absolute address              */
		bool			_addr_off_;		/* address offset                */
		/* immediate meta */
		size_t			_repeat_;		/* repeat of block               */
		/* symbol meta */
		std::string     _label_;		/* label of pseudo machine code  */
		std::string		_symb_;			/* symbol of machine code        */
		std::string		_symb_from_;	/* symbol of machine code (from) */
		std::string		_symb_to_;		/* symbol of machine code (to)   */
		/* platform meta */
};

