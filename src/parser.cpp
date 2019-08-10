/* Copyright (C) 2017-2019 cmj. All right reserved. */

#include "zasm.h"
 
/* ZASM GRAMMAR
	stmt      : ( code | label | define | data | block | include )? [ COMMENT ] NEWLINE
	code      : OPCODE    ( operand )*
	label     : TOKEN     ':' ( decorator )*
	define    : 'define'  TOKEN  ( IMM | STRING )
	data      : 'data'    TOKEN  STRING
	block     : 'block'  STRING  [ IMM | range ]
	include   : 'include' STRING

	operand   : REG | MEM | IMM | reference
	decorator : '@' TOKEN [ ':' TOKEN ]
	reference : '&' TOKEN | '$' | '$$'
	range     : ( IMM | reference ) '~' ( IMM | reference )
*/
void Zasm::parser(std::string line) {
	ZasmToken token;

	this->_lineno_ ++;
	this->_line_buff_ = line;
	this->_cur_lex_ = 0;
	_D(LOG_DEBUG, "%s:L#%04zu", line.c_str(), this->_lineno_);

	token = this->lexer();
	_D(LOG_DEBUG, "Zasm::parse `%s` (#%d) V:" OFF_T, token.raw.c_str(), token.type, this->_vaddr_);
	switch (token.type) {
		case ZASMT_NEWLINE:
			/*empty line */
			break;
		case ZASMT_INCLUDE:
			include_statement(token);
			break;
		case ZASMT_DEFINE:
			define_statement(token);
			break;
		case ZASMT_BLOCK:
			block_statement(token);
			break;
		case ZASMT_DATA:
			data_statement(token);
			break;
		default:
			code_label_statement(token);
			break;
	}
}

/* inner parser logical */
void Zasm::code_label_statement(ZasmToken& opcode) {
	ZasmCode *code = NULL;
	ZasmToken token = this->lexer();
	std::vector<ZasmToken> operands = {};

	switch (token.type) {
		case ZASMT_COLON:	/* label */
			code = new ZasmCode(opcode.raw);
			decorator_expr(code);
			break;
		default:			/* code  */
			while (token.type != ZASMT_NEWLINE) {
				switch (token.type) {
					case ZASMT_REF:
						break;
					default:
						operands.push_back(token);
						break;
				}
				token = this->lexer();
			}

			code = new ZasmCode();
			code->emit(opcode.raw, operands, this->_arch_, this->_vaddr_);
			break;
	}

	(*this) += code;
}

void Zasm::define_statement(ZasmToken& opcode) {
	ZasmToken key = this->lexer(), value;

	if (ZASMT_TOKEN != key.type) {
		_D(LOG_WARN, "define syntax not valid for key %s (%d)", key.raw.c_str(), key.type);
		throw Exception(ERR_SYNTAX_ERROR, "define syntax not valid - " + key.raw);
	}

	value = this->lexer();
	if (! (ZASMT_IMM == value.type || ZASMT_STRING == value.type || ZASMT_TOKEN == value.type) ) {
		_D(LOG_WARN, "define syntax not valid for value %s (%d)", value.raw.c_str(), value.type);
		throw Exception(ERR_SYNTAX_ERROR, "define syntax not valid - " + value.raw);
	}

	this->_defines_[key.raw] = value;
}

void Zasm::include_statement(ZasmToken& token) {
	ZasmToken file = this->lexer();

	if (file.type != ZASMT_STRING) {
		_D(LOG_WARN, "include file is not string - %s (%d)", file.raw.c_str(), file.type);
		throw Exception(ERR_SYNTAX_ERROR, "include syntax not valid - " + file.raw);
	}

	Zasm::assemble( escape(file.raw, false) );
}

void Zasm::block_statement(ZasmToken& token) {
	ZasmCode *code = NULL;

	Zasm::buffer_expr(code);
	token = this->lexer();
	if (ZASMT_NEWLINE != token.type) Zasm::range_expr(code, token);

	(*this) += code;
}

void Zasm::data_statement(ZasmToken& opcode) {
	ZasmToken token = this->lexer();
	ZasmCode *code = NULL;

	if (ZASMT_TOKEN != token.type) {
		_D(LOG_WARN, "data syntax not valid %s (%d)", token.raw.c_str(), token.type);
		throw Exception(ERR_SYNTAX_ERROR, "data syntax not valid - " + token.raw);
	}

	/* data TOKEN BUFFER */
	Zasm::buffer_expr(code, token.raw);
	token = this->lexer();
	if (ZASMT_NEWLINE != token.type) {
		_D(LOG_WARN, "data syntax not valid %s (%d)", token.raw.c_str(), token.type);
		throw Exception(ERR_SYNTAX_ERROR, "data syntax not valid - " + token.raw);
	}

	(*this) += code;
}

/* expression */
void Zasm::decorator_expr(ZasmCode *code) {
	ZasmToken token = this->lexer();

	do {
		if (ZASMT_NEWLINE == token.type) {
			/* end-of-statement */
			break;
		} else if (ZASMT_DECORATOR == token.type) {
			ZasmToken key = this->lexer(), value;

			if (ZASMT_TOKEN != key.type) {
				_D(LOG_WARN, "decorator syntax not valid %s (%d)", key.raw.c_str(), key.type);
				throw Exception(ERR_SYNTAX_ERROR, "decorator syntax not valid - " + key.raw);
			}
			token = this->lexer();

			if (ZASMT_COLON == token.type) {
				value = this->lexer();
				if (! (ZASMT_TOKEN == value.type || ZASMT_IMM == value.type)) {
					_D(LOG_WARN, "decorator syntax not valid %s (%d)", value.raw.c_str(), value.type);
					throw Exception(ERR_SYNTAX_ERROR, "decorator syntax not valid - " + value.raw);
				}

				token = this->lexer();
			}


			/* process the decorator */
			if (key.raw == DECORATOR_REALMODE) {
				this->_arch_ = ARCH_X86_REAL_MODE;
			} else if (key.raw == DECORATOR_PROTMODE) {
				this->_arch_ = ARCH_X86_PROT_MODE;
			} else if (key.raw == DECORATOR_VM_ADDR && value.type == ZASMT_IMM) {
				asInt(value.raw, this->_vaddr_);
				code->set_vaddr(this->_vaddr_);
			} else {
				_D(LOG_CRIT, "Not implemented decorator %s", key.raw.c_str());
				throw Exception(ERR_SYNTAX_ERROR, "Not implemented decorator - `" + key.raw + "`");
			}

			code->set_decorator(key.raw, value.raw);
			continue;
		} else {
			_D(LOG_WARN, "decorator syntax not valid %s (%d)", token.raw.c_str(), token.type);
			throw Exception(ERR_SYNTAX_ERROR, "decorator syntax not valid - " + token.raw);
		}
	} while (1);
}

void Zasm::range_expr(ZasmCode *code, ZasmToken &token) {
	char buf[64] = {0};
	ZasmToken tmp, from = {"0", ZASMT_IMM};

	switch (token.type) {
		case ZASMT_IMM:
			switch ((tmp = this->lexer()).type) {
				case ZASMT_NEWLINE:
					/* set repeat times */
					code->set_repeat( from, token );
					break;
				case ZASMT_RANGE:
					from = token;
					goto SUB_RANGE;
					break;
				default:
					_D(LOG_WARN, "range expression not valid %s (%d)", token.raw.c_str(), token.type);
					throw Exception(ERR_SYNTAX_ERROR, "range expression syntax not valid - " + token.raw);
			}
			break;
		case ZASMT_CUR_POS:
		case ZASMT_CUR_SECTION:
			if (ZASMT_RANGE != (tmp = this->lexer()).type) {
				_D(LOG_WARN, "range expression not valid %s (%d)", tmp.raw.c_str(), tmp.type);
				throw Exception(ERR_SYNTAX_ERROR, "range expression syntax not valid - " + tmp.raw);
			}
			snprintf(buf, sizeof(buf), "0x%lX", token.type == ZASMT_CUR_POS ? this->_len_ : this->_len_ - this->_seclen_);
			from.raw = buf;
			goto SUB_RANGE;
		case ZASMT_REF:
			if (ZASMT_TOKEN != (token = this->lexer()).type) {
				_D(LOG_WARN, "range expression not valid %s (%d)", token.raw.c_str(), token.type);
				throw Exception(ERR_SYNTAX_ERROR, "range expression syntax not valid - " + token.raw);
			}
SUB_RANGE:
			switch ((token = this->lexer()).type) {
				case ZASMT_IMM:
					code->set_repeat( from, token);
					break;
				case ZASMT_CUR_POS:
				case ZASMT_CUR_SECTION:
					snprintf(buf, sizeof(buf), "0x%lX", token.type == ZASMT_CUR_POS ? this->_len_ : this->_len_ - this->_seclen_);
					token.raw = buf;
					token.type = ZASMT_IMM;
					code->set_repeat( from, token);
					break;
				case ZASMT_REF:
					if (ZASMT_TOKEN != (token = this->lexer()).type) {
						_D(LOG_WARN, "range expression not valid %s (%d)", token.raw.c_str(), token.type);
						throw Exception(ERR_SYNTAX_ERROR, "range expression syntax not valid - " + token.raw);
					}
					code->set_repeat( from, token);
					break;
				default:
					_D(LOG_WARN, "range expression not valid %s (%d)", token.raw.c_str(), token.type);
					throw Exception(ERR_SYNTAX_ERROR, "range expression syntax not valid - " + token.raw);
					break;
			}
			token = this->lexer();
			break;
		default:
			_D(LOG_WARN, "range expression not valid %s (%d)", token.raw.c_str(), token.type);
			throw Exception(ERR_SYNTAX_ERROR, "range expression syntax not valid - " + token.raw);
	}
}

void Zasm::buffer_expr(ZasmCode *&code, std::string symb) {
	off_t value;
	std::string src;
	ZasmToken token = this->lexer(), tmp;
	CodeSize size = CODE_OPERAND_SIZE_UNKONWN;

	switch (token.type) {
		case ZASMT_STRING:
			code = new ZasmCode(symb);
			code->set_data(escape(token.raw, false));
			code->set_vaddr(this->_vaddr_);
			break;
		case ZASMT_MEM_SIZE:
			tmp  = Zasm::ref_expr();

			code = new ZasmCode(symb);
			code->set_vaddr(this->_vaddr_);

			if (token.raw == ZASM_MEM_BYTE)		size = CODE_OPERAND_SIZE_8;
			if (token.raw == ZASM_MEM_WORD)		size = CODE_OPERAND_SIZE_16;
			if (token.raw == ZASM_MEM_DWORD)	size = CODE_OPERAND_SIZE_32;
			if (token.raw == ZASM_MEM_QWORD)	size = CODE_OPERAND_SIZE_64;

			code->set_immediate(0, size);

			switch (tmp.type) {
				case ZASMT_IMM:
					BUG(false == asInt(tmp.raw, value));
					code->set_immediate(value + size, 0, 0);
					break;
				case ZASMT_CUR_POS:
				case ZASMT_CUR_SECTION:
				case ZASMT_TOKEN:
					code->set_ref(tmp.raw);
					break;
				default:
					_D(LOG_CRIT, "buffer syntax not valid %s (%d)", token.raw.c_str(), token.type);
					throw Exception(ERR_SYNTAX_ERROR, "buffer syntax not valid - " + token.raw);
					break;
			}
			break;
		default:
			_D(LOG_CRIT, "buffer syntax not valid %s (%d)", token.raw.c_str(), token.type);
			throw Exception(ERR_SYNTAX_ERROR, "buffer syntax not valid - " + token.raw);
	}
}

ZasmToken Zasm::ref_expr(void) {
	ZasmToken token;

	switch ((token = this->lexer()).type) {
		case ZASMT_IMM:
		case ZASMT_CUR_SECTION:
		case ZASMT_CUR_POS:
			/* simple reference */
			break;
		case ZASMT_REF:
			token = this->lexer();
			if (ZASMT_TOKEN != token.type) {
				_D(LOG_WARN, "reference expression not valid %s (%d)", token.raw.c_str(), token.type);
				throw Exception(ERR_SYNTAX_ERROR, "reference expression syntax not valid - " + token.raw);
			}
			break;
		default:
			_D(LOG_WARN, "reference expression not valid %s (%d)", token.raw.c_str(), token.type);
			throw Exception(ERR_SYNTAX_ERROR, "reference expression syntax not valid - " + token.raw);
	}

	return token;
}
