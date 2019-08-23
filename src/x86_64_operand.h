/* Copyright (C) 2017-2019 cmj. All right reserved. */
#pragma once

/* x86 operand converter */
class Operand {
	public:
		Operand(void) : _token_(ZasmToken({"", ZASMT_UNKONWN})), _size_(CODE_OPERAND_SIZE_UNKONWN), _type_(ZASMT_UNKONWN) {
			this->_reg_seg_ = false;
			this->_reg_ctl_ = false;
			this->_reg_dbg_ = false;
			this->_reg_ext_ = false;
			this->_reg_pos_ = 0;
			this->_seg_pos_ = 0;

			this->_reg_highbyte_	= false;

			this->_mem_seg_		= NULL;
			this->_mem_index_	= NULL;
			this->_mem_based_	= NULL;
			this->_mem_scalar_	= CODE_OPERAND_SIZE_UNKONWN;
			this->_mem_offset_	= 0x00;

			this->_rel_first	= NULL;
			this->_rel_second	= NULL;
		};
		Operand(ZasmToken &token) : _token_(token), _size_(CODE_OPERAND_SIZE_UNKONWN), _type_(token.type) {
			this->_reg_seg_ = false;
			this->_reg_ctl_ = false;
			this->_reg_dbg_ = false;
			this->_reg_ext_ = false;
			this->_reg_pos_ = 0;
			this->_seg_pos_ = 0;

			this->_reg_highbyte_	= false;

			this->_mem_seg_		= NULL;
			this->_mem_index_	= NULL;
			this->_mem_based_	= NULL;
			this->_mem_scalar_	= CODE_OPERAND_SIZE_UNKONWN;
			this->_mem_offset_	= 0x00;

			this->_rel_first	= NULL;
			this->_rel_second	= NULL;

			switch (this->_type_) {
				case ZASMT_REG:
					this->parse_reg(token.raw);
					break;
				case ZASMT_IMM:
					/* need not process */
					break;
				case ZASMT_MEM:
					this->parse_mem(token.raw);
					break;
				case ZASMT_TOKEN: case ZASMT_CUR_POS: case ZASMT_CUR_SECTION:
					this->_type_ = ZASMT_REF;
					/* reference */
					break;
				default:
					_D(LOG_CRIT, "Invalid token %s", token.raw.c_str());
					throw Exception(ERR_INVALID_TOKEN, "Not valid token `" + token.raw + "`");
					break;
			}
		}
		Operand(ZasmToken &first, ZasmToken &second) {
			this->_token_	= ZasmToken({first.raw + ":" + second.raw, ZASMT_ADDR_OFF});
			this->_type_	= ZASMT_ADDR_OFF;
			this->_size_	= CODE_OPERAND_SIZE_32;

			this->_reg_seg_ = false;
			this->_reg_ctl_ = false;
			this->_reg_dbg_ = false;
			this->_reg_ext_ = false;
			this->_reg_pos_ = 0;
			this->_seg_pos_ = 0;

			this->_reg_highbyte_	= false;

			this->_mem_seg_		= NULL;
			this->_mem_index_	= NULL;
			this->_mem_based_	= NULL;
			this->_mem_scalar_	= CODE_OPERAND_SIZE_UNKONWN;
			this->_mem_offset_	= 0x00;

			this->_rel_first	= new Operand(first);
			this->_rel_second	= new Operand(second);
		}


		/* check function */
		bool match(long flag) {
			bool blRet = false;
			off_t value = 0x00;

			switch (this->_type_) {
				case ZASMT_UNKONWN:
					blRet = flag == CODE_NONE;
					break;
				case ZASMT_REG:
					switch (flag & CODE_BS_MASK) {
						case CODE_REGISTER:					/* general register         */
						case CODE_REGISTER | CODE_MEMORY:	/* both register and memory */
							blRet = !this->_reg_seg_ && !this->_reg_ctl_ && !this->_reg_dbg_;
							break;
						case CODE_REG_SEG:
							blRet = this->_reg_seg_;
							break;
						case CODE_REG_CTR:
							blRet = this->_reg_ctl_;
							break;
						case CODE_REG_DBG:
							blRet = this->_reg_dbg_;
							break;
						case CODE_REG_SPECIFIED:
							blRet = ! (this->_reg_seg_ || this->_reg_dbg_ || this->_reg_ctl_ || this->_reg_ext_);
							blRet = blRet && (this->_reg_pos_ == (flag >> CODE_CONST_OFF));
							break;
						default:
							blRet = false;
							break;
					}

					blRet = blRet && this->match_size(flag);
					break;
				case ZASMT_MEM:
					blRet = (0 != (flag & CODE_MEMORY)) && match_size(flag);
					break;
				case ZASMT_IMM:
					switch (flag & CODE_BS_MASK) {
						case CODE_IMMEDIATE:
						case CODE_RELATED:
							blRet = true;
							break;
						case CODE_CONSTANT:
							switch ( flag ) {
								case CODE_CONSTANT_1:
									asInt(this->_token_.raw, value);
									blRet = 1 == value;
									break;
								case CODE_CONSTANT_3:
									asInt(this->_token_.raw, value);
									blRet = 3 == value;
									break;
								default:
									_D(LOG_CRIT, "Not implement %lX", flag);
									throw Exception(ERR_NOT_IMPLEMENTED, "Not implement");
							}
							break;
						default:
							blRet = false;
							break;
					}
					break;
				case ZASMT_ADDR_OFF:
					blRet = CODE_ADDRESS_OFF == (flag & CODE_ADDRESS_OFF);
					break;
				case ZASMT_REF:
					if (this->_token_.type == ZASMT_CUR_POS || this->_token_.type == ZASMT_CUR_SECTION) {
						blRet = 0 != (flag & CODE_RELATED);
					}
					/* reference */
					blRet = blRet || (0 != (flag & (CODE_MEMORY | CODE_IMMEDIATE | CODE_RELATED)));
					blRet = blRet && (CODE_ADDRESS_OFF != (flag & CODE_ADDRESS_OFF));
					break;
				default:
					throw Exception(ERR_NOT_IMPLEMENTED, "Not implement match on `" + this->_token_.raw + "`");
					break;
			}

			return blRet;
		}

		bool match_size(long flag) {
			bool blRet = false;

			switch (this->_size_) {
				case CODE_OPERAND_SIZE_8:
					blRet = 0 != (flag & CODE_SIZE_MASK & CODE_SIZE8);
					break;
				case CODE_OPERAND_SIZE_16:
					blRet = 0 != (flag & CODE_SIZE_MASK & CODE_SIZE16);
					break;
				case CODE_OPERAND_SIZE_32:
					blRet = 0 != (flag & CODE_SIZE_MASK & CODE_SIZE32);
					break;
				case CODE_OPERAND_SIZE_64:
					blRet = 0 != (flag & CODE_SIZE_MASK & CODE_SIZE64);
					break;
				default:
					/* NOP */
					switch (flag) {
						case CODE_REG_SEG:
							blRet = this->_reg_seg_;
							break;
						case CODE_REG_CTR:
							blRet = this->_reg_ctl_;
							break;
						case CODE_REG_DBG:
							blRet = this->_reg_dbg_;
							break;
						default:
							_D(LOG_CRIT, "Unknown size #%d", this->_size_);
							break;
					}
					break;
			}

			return blRet;
		}

		/* operand parser */
		void parse_reg(std::string reg) {
			static std::vector<std::string> reg_seg = { X86_REGISTER_SEG };
			static std::vector<std::string> reg_ctl = { CTRL_REGISTER };
			static std::vector<std::string> reg_dbg = { DEBUG_REGISTER };
			static std::vector<std::string> reg_ext = { X86_REGISTER_EXT };
			static std::vector<std::string> reg_gen = { X86_GENERAL_REG };
			std::vector<std::string>::iterator it;

			if (reg_seg.end() != (it = std::find(reg_seg.begin(), reg_seg.end(), reg))) {
				this->_reg_seg_ = true;
				this->_reg_pos_ = it - reg_seg.begin();
				this->_seg_pos_ = this->_reg_pos_;
			} else if (reg_ctl.end() != (it = std::find(reg_ctl.begin(), reg_ctl.end(), reg))) {
				this->_reg_ctl_ = true;
				this->_reg_pos_ = it - reg_seg.begin();
			} else if (reg_dbg.end() != (it = std::find(reg_dbg.begin(), reg_dbg.end(), reg))) {
				this->_reg_ext_ = false;
				this->_reg_pos_ = it - reg_dbg.begin();
			} else if (reg_ext.end() != (it = std::find(reg_ext.begin(), reg_ext.end(), reg))) {
				this->_reg_ext_ = true;
				this->_reg_pos_ = (it - reg_ext.begin()) % 8;

				switch ((it - reg_ext.begin()) / 8) {
					case 0:
						this->_size_ = CODE_OPERAND_SIZE_64;
						break;
					case 1:
						this->_size_ = CODE_OPERAND_SIZE_32;
						break;
					case 2:
						this->_size_ = CODE_OPERAND_SIZE_16;
						break;
					case 3:
						this->_size_ = CODE_OPERAND_SIZE_8;
						break;
					default:
						throw Exception(ERR_INVALID_TOKEN, "Invalid register token `" + this->_token_.raw + "`");
						break;
				}
			} else if (reg_gen.end() != (it = std::find(reg_gen.begin(), reg_gen.end(), reg))) {
				this->_reg_pos_ 		= (it - reg_gen.begin()) % 8;
				this->_reg_highbyte_	= ((it - reg_gen.begin()) /8 ) == 4;

				switch ((it - reg_gen.begin()) / 8) {
					case 0:
						this->_size_ = CODE_OPERAND_SIZE_64;
						break;
					case 1:
						this->_size_ = CODE_OPERAND_SIZE_32;
						break;
					case 2:
						this->_size_ = CODE_OPERAND_SIZE_16;
						break;
					case 3:
					case 4:
						this->_size_ = CODE_OPERAND_SIZE_8;
						break;
					default:
						_D(LOG_CRIT, "Not a valid register `%s` %zu", this->_token_.raw.c_str(), (it - reg_gen.begin()) / 8);
						throw Exception(ERR_INVALID_TOKEN, "Invalid register token `" + this->_token_.raw + "`");
						break;
				}
			} else {
				_D(LOG_CRIT, "Not a valid register `%s`", reg.c_str());
				throw Exception(ERR_INVALID_TOKEN, "Not a valid register `" + reg + "`");
			}

			_D(LOG_DEBUG, "reg %-4s (%d) P:%d S:%c C:%c D:%c E:%c H:%c",
					reg.c_str(),
					this->_size_,
					this->_reg_pos_,
					this->_reg_seg_ ? 'T' : 'F',
					this->_reg_ctl_ ? 'T' : 'F',
					this->_reg_dbg_ ? 'T' : 'F',
					this->_reg_ext_ ? 'T' : 'F',
					this->_reg_highbyte_ ? 'T' : 'F'
			);
		}

		/* x86-family memory : [ segment : scalar * index register + based register + offset ] */
		void parse_mem(std::string mem) {
			bool neg = false;
			size_t i, j;
			off_t tmp_value = 0;
			std::string tmp;
			Operand *next = NULL;

			BUG('[' != mem[0] || ']' != mem[ mem.length()-1 ]);

			for (i = 1; i < mem.length()-1; ++i) {
				switch (mem[i]) {
					case ' ': case '\t':
						/* NOP */
						break;
					case '+': case '-':
						neg = '-' == mem[i];
						break;
					case '*':
						if (CODE_OPERAND_SIZE_UNKONWN == this->_mem_scalar_) {
							switch(this->_mem_offset_) {
								case CODE_OPERAND_SIZE_8:
								case CODE_OPERAND_SIZE_16:
								case CODE_OPERAND_SIZE_32:
								case CODE_OPERAND_SIZE_64:
									this->_mem_scalar_ = (CodeSize)this->_mem_offset_;
									break;
								default:
									_D(LOG_CRIT, "Invalid scalar in memory token - %s", mem.c_str());
									throw Exception(ERR_INVALID_TOKEN, "Invalid scalar in memory token - " + mem);
									break;
							}
							break;
						}
						_D(LOG_CRIT, "Multiple scalar in memory token - %s", mem.c_str());
						throw Exception(ERR_INVALID_TOKEN, "Multiple scalar in memory token - " + mem);
					case ':':
						this->_mem_seg_  = this->_mem_based_;
						this->_mem_based_ = NULL;
						if (! this->_mem_seg_->_reg_seg_) {
							_D(LOG_CRIT, "Invalid segment register in memory - %s", mem.c_str());
							throw Exception(ERR_INVALID_TOKEN, "Invalid segment register in memory - " + mem);
						}
						break;
					default:
						for (j = i; j < mem.length()-1; ++j) {
							if ((mem[j] | 0x20) >= 'a' && (mem[j] | 0x20) <= 'z') continue;
							if (mem[j] >= '0' && mem[j] <= '9') continue;
							break;
						}

						tmp = mem.substr(i, j-i);
						if (asInt(tmp, tmp_value)) {
							/* scalar or offset */
							this->_mem_offset_ = tmp_value * (neg ? -1 : 1);
							neg = false;
						} else {
							next = new Operand();
							next->parse_reg(tmp);

							if (NULL == this->_mem_index_ && CODE_OPERAND_SIZE_UNKONWN != this->_mem_scalar_) {
								/* index register */
								this->_mem_index_ = next;
							} else if (NULL == this->_mem_based_) {
								/* based register */
								this->_mem_based_ = next;
							} else if (NULL == this->_mem_index_) {
								/* index register */
								this->_mem_index_ = next;
							} else {
								_D(LOG_CRIT, "Invalid memory token - %s (%s)", tmp.c_str(), mem.c_str());
								throw Exception(ERR_INVALID_TOKEN, "Invalid token - " + mem);
							}
						}
						i = j - 1;
						break;
				}
			}

			this->_size_    = CODE_OPERAND_SIZE_64;
			this->_reg_pos_ = this->_mem_based_ ? this->_mem_based_->_reg_pos_ : 0x00;
			this->_reg_ext_ = (this->_mem_based_ ? this->_mem_based_->_reg_ext_ : false);

			_D(LOG_DEBUG_X86_EMIT, "mem %-22s (%d) P:%d S:%c B:%c S:%d I:%c O:%c E:%c",
				this->_token_.raw.c_str(),
				this->_size_,
				this->_reg_pos_,
				this->_mem_seg_ ? 'T' : 'F',
				this->_mem_based_ ? 'T' : 'F',
				this->_mem_scalar_,
				this->_mem_index_ ? 'T' : 'F',
				this->_mem_offset_ ? 'T' : 'F',
				this->_reg_ext_ ? 'T' : 'F'
			);
		}

		void set_size(ZasmToken &token) {
			if (token.type != ZASMT_MEM_SIZE) {
				_D(LOG_CRIT, "Invalid token %s", token.raw.c_str());
				throw Exception(ERR_INVALID_TOKEN, "Invalid token - " + token.raw);
			}

			if (token.raw == ZASM_MEM_BYTE) {
				this->_size_ = CODE_OPERAND_SIZE_8;
			} else if (token.raw == ZASM_MEM_WORD) {
				this->_size_ = CODE_OPERAND_SIZE_16;
			} else if (token.raw == ZASM_MEM_DWORD) {
				this->_size_ = CODE_OPERAND_SIZE_32;
			} else if (token.raw == ZASM_MEM_QWORD) {
				this->_size_ = CODE_OPERAND_SIZE_64;
			}
		}

		/* meta for register */
		bool _reg_seg_;			/* segment register      */
		bool _reg_ctl_;			/* control register      */
		bool _reg_dbg_;			/* debug register        */
		bool _reg_ext_;			/* extension register    */
		int  _reg_pos_;			/* position of register  */
		int  _seg_pos_;			/* position of segment   */
		bool _reg_highbyte_;	/* high byte of register */

		/* meta for memory : [ segment : scalar * index + based + offset ]*/
		Operand *_mem_seg_;		/* register of segment */
		Operand *_mem_index_;	/* register of index   */
		Operand *_mem_based_;	/* register of based   */
		CodeSize _mem_scalar_;	/* scalar of index     */
		off_t    _mem_offset_;	/* offset of memory    */

		/* related address : X:Y */
		Operand *_rel_first;
		Operand *_rel_second;

		ZasmToken 		_token_;
		CodeSize  		_size_;
		ZasmTokenType	_type_;
};

