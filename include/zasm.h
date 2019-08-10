/* Copyright (C) 2017-2019 cmj. All right reserved. */
#pragma once

#define ZASM_VER_MAJOR		1
#define ZASM_VER_MINOR		0


#define ZASM_DEFINE			"define"
#define ZASM_DATA			"data"
#define ZASM_INCLUDE		"include"
#define ZASM_BLOCK			"block"
#define ZASM_MEM_BYTE		"byte"
#define ZASM_MEM_WORD		"word"
#define ZASM_MEM_DWORD		"dword"
#define ZASM_MEM_QWORD		"qword"
#define ZASM_CUR_POS		"$"
#define ZASM_CUR_SECTION	"$$"

#define ZASM_DEFAULT_OUTPUT	"a.out"


#include <string>
#include <set>
#include <vector>
#include <map>

typedef enum {
	ZASMT_UNKONWN	= 0,
	ZASMT_NEWLINE,
	ZASMT_REG,
	ZASMT_MEM,
	ZASMT_IMM,
	ZASMT_STRING	= 5,
	ZASMT_TOKEN,
	ZASMT_MEM_SIZE,
	ZASMT_DEFINE,			/* define  */
	ZASMT_DATA,				/* data    */
	ZASMT_INCLUDE	= 10,	/* include */
	ZASMT_BLOCK,			/* block  */
	ZASMT_DECORATOR,		/* @       */
	ZASMT_CUR_POS,			/* $       */
	ZASMT_CUR_SECTION,		/* $$      */
	ZASMT_REF		= 15,	/* &       */
	ZASMT_COLON,			/* :       */
	ZASMT_RANGE,			/* ~       */
	ZASMT_ADDR_OFF,			/* XXX:XXX */
} ZasmTokenType;

typedef struct {
	std::string raw;
	ZasmTokenType type;
} ZasmToken;

typedef enum {
	ZASM_FMT_BIN	= 0x00,
	ZASM_FMT_ELF64,
	ZASM_FMT_MACH_O_64,
} ZasmBinaryFormat;

#include "debug.h"
#include "utility.h"
#include "zasm/code.h"
#include "zasm/platform.h"
#include "zasm/register.h"

class Zasm {
	public:
		Zasm(std::string fmt);
		Zasm(ZasmBinaryFormat fmt) : _fmt_(fmt) {};
		virtual ~Zasm();

		void assemble(std::string srcfile);
		size_t lineno(void)	{ return this->_lineno_; }
	protected:
		void parser(std::string srcline);
		ZasmToken lexer();

		void code_label_statement(ZasmToken& token);
		void include_statement(ZasmToken& token);
		void define_statement(ZasmToken& token);
		void data_statement(ZasmToken& token);
		void block_statement(ZasmToken& token);

		void decorator_expr(ZasmCode *code);
		void range_expr(ZasmCode *code, ZasmToken &token);
		void buffer_expr(ZasmCode *&code, std::string symb="");
		ZasmToken ref_expr(void);

		/* generate the binary file */
		off_t dumpfile_bin(std::fstream &st);
		off_t dumpfile_elf64(std::fstream &st);
		off_t dumpfile_macho64(std::fstream &st);

		friend Zasm& operator+= (Zasm& zasm, ZasmCode *code);
		friend std::fstream& operator << (std::fstream &st, Zasm &src);
	private:
		size_t _cur_lex_, _lineno_, _len_, _seclen_;
		std::string _line_buff_;
		std::vector<size_t> _lineno_list_;
		std::set<std::string> _srclist_;
		std::map<std::string, ZasmToken> _defines_;
		std::map<std::string, size_t> _symbols_;

		std::vector<ZasmCode *> _codes_;
		off_t 			_vaddr_;		/* current virtual address       */
		ZasmCodeArch	_arch_;			/* related CPU architecture      */

		/* final binary format */
		bool _string_;
		ZasmBinaryFormat _fmt_;
};

