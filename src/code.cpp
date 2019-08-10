/* Copyright (C) 2017-2019 cmj. All right reserved. */

#include <cstring>
#include <fstream>
#include <vector>

#include "zasm.h"

ZasmCode::ZasmCode(void) {
	this->_code_      = NULL;
	this->_len_       = 0x00;
	this->_imm_len_   = CODE_OPERAND_SIZE_UNKONWN;
	this->_absaddr_   = false;
	this->_addr_off_  = false;
	this->_repeat_    = 0x01;
	this->_vaddr_     = 0x00;
	this->_undefined_ = false;
}

ZasmCode::ZasmCode(std::string symb) {
	this->_label_     = symb;
	this->_code_      = NULL;
	this->_len_       = 0x00;
	this->_absaddr_   = false;
	this->_addr_off_  = false;
	this->_repeat_    = 0x01;
	this->_vaddr_     = 0x00;
	this->_fmt_       = symb + ":";
	this->_undefined_ = false;
}

ZasmCode::~ZasmCode(void) {
	if (NULL != _code_) {
		free( _code_);
		_code_ = NULL;
	}
}

void ZasmCode::set_ref(std::string ref) {
	this->_undefined_ = true;
	this->_absaddr_   = true;
	this->_symb_      = ref;
	this->_fmt_       = "" == this->_symb_ ? (ZASM_BLOCK) : (std::string(ZASM_DATA) + " " + this->_symb_);
}
void ZasmCode::set_data(std::string escaped_string) {
	this->_len_  = escaped_string.size();
	this->_code_ = (unsigned char *)malloc(sizeof(unsigned char) * (this->_len_));
	this->_fmt_  = "" == this->_symb_ ? (ZASM_BLOCK) : (std::string(ZASM_DATA) + " " + this->_symb_);
	for (size_t i = 0; i < this->_len_; ++i) {
		this->_code_[i] = (unsigned char)escaped_string[i];
	}
}

void ZasmCode::set_repeat(ZasmToken &from, ZasmToken &to) {
	off_t from_imm, to_imm;

	if (ZASMT_IMM == from.type && ZASMT_IMM == to.type) {
		BUG(false == asInt(from.raw, from_imm));
		BUG(false == asInt(to.raw, to_imm));

		this->_repeat_ = to_imm - from_imm;
		if (this->_repeat_ & ~ 0xFFFFFF) {
			_D(LOG_CRIT, "Not support range with repeat too many times 0x%lX", this->_repeat_);
			throw Exception(ERR_SYNTAX_ERROR, "Not support range with repeat too many times");
		}
	} else {
		this->_undefined_ = true;
		this->_symb_from_ = from.raw;
		this->_symb_to_   = to.raw;
	}
}

void ZasmCode::set_immediate(off_t value, CodeSize size) {
	this->_imm_len_ = size ? size : this->_imm_len_;

	_D(LOG_DEBUG, "set immediate " OFF_T " S:%d L:%zu", value, this->_imm_len_, this->_len_);

	if (NULL == this->_code_) {
		this->_len_  = 0;
		this->_code_ = (unsigned char *)malloc(sizeof(unsigned char) * (size));
		this->_fmt_  = "" == this->_symb_ ? (ZASM_BLOCK) : (std::string(ZASM_DATA) + " " + this->_symb_);
	}

	for (size_t i = 0; i < this->_imm_len_; ++i) {
		this->_code_[ this->_len_++ ] = value & 0xFF;
		value >>= 8;
	}
}

void ZasmCode::set_immediate(off_t sym_pos, off_t seclen, off_t curlen) {
	_D(LOG_DEBUG, "reset immediate %02X A:%c O:%c V: " OFF_T " C:" OFF_T " S:" OFF_T " V:" OFF_T,
			(unsigned char)this->_code_[0],
			this->_absaddr_ ? 'T': 'F',
			this->_addr_off_ ? 'T' : 'F',
			sym_pos, curlen, seclen, this->_vaddr_
	);

	sym_pos = (this->_absaddr_) ? (sym_pos + this->_vaddr_) : (sym_pos - curlen - this->length());
	this->_len_      -= this->_imm_len_ * (this->_addr_off_ ? 2 : 1);
	this->_undefined_ = false;
	ZasmCode::set_immediate(sym_pos, this->_imm_len_);
	if (curlen && this->_addr_off_) this->_len_ += this->_imm_len_;
}

void ZasmCode::set_decorator(std::string key, std::string value) {
	char buf[BUFSIZ] = {0};

	snprintf(buf, sizeof(buf), "    @%s%s", key.c_str(), (value == "" ? value : ":" + value).c_str());

	this->_fmt_ += buf;
}

std::fstream& operator << (std::fstream &st, const ZasmCode &code) {
	char buf[BUFSIZ] = {0};
	BUG( code._repeat_ & (size_t)~0xFFFF);


	snprintf(buf, sizeof(buf), code._len_ ? "%-52s - " : "%s", code._fmt_.c_str());
	for (size_t i = 0; i < code._len_; ++i) {
		snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%02X ", *(unsigned char *)(&code._code_[i]));
	}

	for (size_t i = 0; i < code._repeat_; ++i) {
		if (0 != code._len_) {
			/* only dump the machine code */
			st.write( (char *)code._code_, code._len_ );
		}
	}

	_D(LOG_DISASSEMBLE, "%s", buf);
	return st;
}

