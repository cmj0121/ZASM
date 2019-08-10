/* Copyright (C) 2017-2019 cmj. All right reserved. */
#pragma once

extern int __verbose__;

#include <stdlib.h>

#define _D(lv, msg, ...)									\
	do {													\
		if (lv <= __verbose__) {							\
			fprintf(stderr, "%-22s L#%04d %10s  " msg "\n",	\
				__FILE__, __LINE__, #lv, ##__VA_ARGS__);	\
		}													\
	} while (0)
#define BUG( expr )								\
	do {										\
		if ( expr ) {							\
			_D(LOG_CRIT, "BUG - %s", #expr);	\
			exit(-1);							\
		}										\
	} while (0)

#define FREE(ctx)		if (NULL != (ctx)) { free(ctx); (ctx) = NULL; }
#define DELETE(ctx)		if (NULL != (ctx)) { delete (ctx); (ctx) = NULL; }
#define ABS(ctx)		((ctx) < 0 ? - (ctx) : (ctx))

typedef enum _tag_log_level_ {
	LOG_CRIT		= 0,
	LOG_BUG			= 1,
	LOG_DISASSEMBLE	= 1,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG,
} LOG_LEVEL;

typedef enum {
	ERR_UNKNOWN,
	ERR_NOT_IMPLEMENTED,
	ERR_IO_ERROR,
	ERR_INVALID_TOKEN,
	ERR_SYNTAX_ERROR,
	ERR_SYMBOL_NOT_DEFINED,
} ExceptionCode;

class Exception {
	public:
		Exception(ExceptionCode code, const std::string msg) throw() :
			_code_(code), _msg_(msg) {};

		virtual char const* what() const throw() { return this->_msg_.c_str(); };

		int code(void) { return this->_code_; }
	private:
		ExceptionCode _code_;
		std::string _msg_;
};
