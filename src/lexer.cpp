/* Copyright (C) 2017-2019 cmj. All right reserved. */

#include <string>
#include <algorithm>

#include "zasm.h"

#define LOG_DEBUG_ZASM_LEXER	10

ZasmToken Zasm::lexer() {
	size_t end = 0;
	std::string tmp = "";
	ZasmTokenType type = ZASMT_UNKONWN;
	static std::vector<std::string> regs = { REGISTERS };

	_D(LOG_DEBUG_ZASM_LEXER, "Zasm::lexer `%s` on #%zu", this->_line_buff_.c_str(), this->_cur_lex_);
	for (; _cur_lex_ <= this->_line_buff_.size(); ++_cur_lex_) {
		switch (this->_line_buff_[_cur_lex_]) {
			case ' ': case '\t':
				/* white space */
				continue;
			case '#': case '\n': case '\0':
				/* newline */
				tmp  = "\n";
				type = ZASMT_NEWLINE;
				_cur_lex_ = this->_line_buff_.size();
				goto END;
			case '\'': case '\"':
				for (end = _cur_lex_+1; end < this->_line_buff_.size(); ++end) {
					if (this->_line_buff_[_cur_lex_] == this->_line_buff_[end]) {
						break;
					} else if ('\\' == this->_line_buff_[end]) {
						end ++;
						if ('x' == this->_line_buff_[end]) ++end;
					}
				}

				if (end >= this->_line_buff_.size()) {
					_D(LOG_BUG, "invalid token `%s`", this->_line_buff_.c_str());
					throw Exception(ERR_INVALID_TOKEN, this->_line_buff_.substr(_cur_lex_));
				}
				end ++;
				tmp  = this->_line_buff_.substr(_cur_lex_, end-_cur_lex_);
				type = ZASMT_STRING;
				_cur_lex_ = end;
				goto END;
			case '[':
				for (end = _cur_lex_+1; end < this->_line_buff_.size(); ++end) {
					if (']' == this->_line_buff_[end]) break;
				}

				if (end >= this->_line_buff_.size()) {
					_D(LOG_BUG, "invalid token `%s`", this->_line_buff_.c_str());
					throw Exception(ERR_INVALID_TOKEN, this->_line_buff_.substr(_cur_lex_));
				}
				end ++;
				tmp  = this->_line_buff_.substr(_cur_lex_, end-_cur_lex_);
				/* remove white-space */
				for (auto it = tmp.begin(); it < tmp.end();) {
					if (*it == ' ' || *it == '\t') {
						tmp.erase(it);
						continue;
					}
					++it;
				}

				type = ZASMT_MEM;
				_cur_lex_ = end;
				goto END;
			case ']':
				_D(LOG_CRIT, "invalid token `%s`", this->_line_buff_.c_str());
				throw Exception(ERR_INVALID_TOKEN, this->_line_buff_.substr(_cur_lex_));
				break;
			case '@': case '~': case '&': case ':':
				tmp = this->_line_buff_[_cur_lex_];
				switch (this->_line_buff_[_cur_lex_]) {
					case '@':
						type = ZASMT_DECORATOR;
						break;
					case '~':
						type = ZASMT_RANGE;
						break;
					case '&':
						type = ZASMT_REF;
						break;
					case ':':
						type = ZASMT_COLON;
						break;
				}
				_cur_lex_ ++;
				goto END;
			case '$':
				for (end = _cur_lex_+1; end < this->_line_buff_.size(); ++end) {
					if (this->_line_buff_[_cur_lex_] != this->_line_buff_[end]) break;
				}

				if (end -_cur_lex_ > 2) {
					_D(LOG_BUG, "invalid token `%s`", this->_line_buff_.c_str());
					throw Exception(ERR_INVALID_TOKEN, this->_line_buff_.substr(_cur_lex_));
				}
				tmp  = this->_line_buff_.substr(_cur_lex_, end-_cur_lex_);
				type = "$" == tmp ? ZASMT_CUR_POS : ZASMT_CUR_SECTION;
				_cur_lex_ = end;
				goto END;
			default:
				for (end = _cur_lex_; end < this->_line_buff_.size(); ++end) {
					bool found = false;

					switch ( this->_line_buff_[end]) {
						case ' ':  case '\t': case '\n':
						case ':':  case '@':  case '~':
						case '\'': case '\"':
						case '[':  case ']':
						case '$':
							found = true;
							break;
						default:
							break;
					}
					if (found) break;
				}

				tmp = this->_line_buff_.substr(_cur_lex_, end-_cur_lex_);
				/* identify the token */
				if (tmp == ZASM_MEM_BYTE || tmp == ZASM_MEM_WORD || tmp == ZASM_MEM_DWORD || tmp == ZASM_MEM_QWORD) {
					type = ZASMT_MEM_SIZE;
				} else if (tmp == ZASM_INCLUDE) {
					type = ZASMT_INCLUDE;
				} else if (tmp == ZASM_DEFINE) {
					type = ZASMT_DEFINE;
				} else if (tmp == ZASM_BLOCK) {
					type = ZASMT_BLOCK;
				} else if (tmp == ZASM_DATA) {
					type = ZASMT_DATA;
				} else if (regs.end() != std::find(regs.begin(), regs.end(), tmp)) {
					type = ZASMT_REG;
				} else {
					off_t x;
					type = asInt(tmp, x) ? ZASMT_IMM : ZASMT_TOKEN;
				}
				_cur_lex_ = end;
				goto END;
		}
	}
END:
	BUG ( "" == tmp);

	if (1 == this->_defines_.count(tmp)) {
		_D(LOG_INFO, "replace token %s -> %s", tmp.c_str(), this->_defines_[tmp].raw.c_str());
		return this->_defines_[tmp];
	}

	_D(LOG_DEBUG_ZASM_LEXER, "Zasm::lexer token `%s`", tmp.c_str());
	return ZasmToken({
		.raw	= tmp,
		.type	= type
	});
}
