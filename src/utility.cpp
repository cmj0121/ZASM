/* Copyright (C) 2017-2019 cmj. All right reserved. */

#include <string>

#include "utility.h"

std::string escape(std::string src, bool raw_string) {
	char ch = 0x00;
	std::string tmp = src, dst = "";

	if ( ! raw_string ) {
		src = src.substr(1, src.length() - 2);
	}

	for (size_t i = 0; i < src.length(); ++i) {
		if (src[i] == '\\') {
			if ((++i) > src.length()) {
				_D(LOG_DEBUG, "token `%s` is invalid in position %zu", src.c_str(), i);
				throw Exception(ERR_INVALID_TOKEN, "token `" + src + "` is invalid");
			}

			switch (src[i]) {
				case 'a':
					dst += '\a';
					break;
				case 'b':
					dst += '\b';
					break;
				case 'f':
					dst += '\f';
					break;
				case 'n':
					dst += '\n';
					break;
				case 'r':
					dst += '\r';
					break;
				case 't':
					dst += '\t';
					break;
				case 'v':
					dst += '\v';
					break;
				case '0':
					dst += '\0';
					break;
				case '\\':
					dst += '\\';
					break;
				case '\'':
					dst += '\'';
					break;
				case '"':
					dst += '"';
					break;
				case 'x':
					ch = 0x00;
					for (int j = 0; j < 2; ++j) {
						++i;
						if (src[i] >= '0' && src[i] <= '9') {
							ch = ch * 16 + (src[i] - '0');
						} else if ((src[i] | 0x20) >= 'a' && (src[i] | 0x20) <= 'f') {
							ch = ch * 16 + ((src[i] | 0x20) - 'a') + 0x0A;
						} else {
							_D(LOG_DEBUG, "token `%s` is invalid in position %zu", src.c_str(), i);
							throw Exception(ERR_INVALID_TOKEN, "token `" + src + "` is invalid");
						}
					}
					dst += ch;
					break;
				default:
					_D(LOG_DEBUG, "token `%s` is invalid in position %zu", src.c_str(), i);
					throw Exception(ERR_INVALID_TOKEN, "token `" + src + "` is invalid");
					break;
			}

			continue;
		}

		dst += src[i];
	}

	#if 0
	tmp = src +  " -> ";
	for (size_t i = 0; i < dst.length(); ++i) {
		char buff[6] = {0};
		snprintf(buff, sizeof(buff), "0x%02X ", (unsigned char)dst[i]);
		tmp += buff;
	}
	_D(LOG_CRIT, "escape string : %s", tmp.c_str());
	#endif

	return dst;
}

typedef enum {
	RADIX_ERROR	= 0,
	RADIX_2		= 2,
	RADIX_8		= 8,
	RADIX_10	= 10,
	RADIX_16	= 16,
} Radix;

bool asInt(std::string src, off_t &ret) {
	bool valid = false;
	Radix radix = RADIX_ERROR;

	/* reset */
	ret = 0;
	for (size_t i = 0; i < src.length(); ++i) {
		switch ( src[i] ) {
			case '0':
				if (i == 0) {
					radix = RADIX_8;
					break;
				}
			case '1':
				/* binary */
				if (radix == RADIX_2) {
					ret = ret * radix + (src[i] - '0');
					break;
				}
			case '2': case '3': case '4': case '5': case '6': case '7':
				/* octal */
				if (radix == RADIX_8) {
					ret = ret * radix + (src[i] - '0');
					break;
				}
			case '8': case '9':
				/* digit */
				if ((i == 0 && radix == RADIX_ERROR) || radix == RADIX_10 || radix == RADIX_16) {
					ret = ret * radix + (src[i] - '0');
					radix = i == 0 ? RADIX_10 : radix;
					break;
				}
			case 'b': case 'B':
				if (i == 1 && 2 != src.length()) {
					radix = RADIX_2;
					break;
				}
			case 'a': case 'c': case 'd': case 'e': case 'f':
			case 'A': case 'C': case 'D': case 'E': case 'F':
				if (radix == RADIX_16) {
					/* hexadecimal */
					ret = ret * radix + ((src[i] | 0x20) - 'a') + 0x0A;
					break;
				}
			case 'o': case 'O':
				if (i == 1 && 2 != src.length()) {
					radix = RADIX_8;
					break;
				}
			case 'x': case 'X':
				if (i == 1 && 2 != src.length()) {
					radix = RADIX_16;
					break;
				}
			default:
				if (i == 0) goto END;

				_D(LOG_BUG, "invalid token `%s`", src.c_str());
				throw Exception(ERR_INVALID_TOKEN, "invalid token `" + src + "`");
		}
	}
	valid = true;
END:
	_D(LOG_DEBUG, "%s (" OFF_T ") %s digit", src.c_str(), ret, valid ? "is" : "is not");
	return valid;
}

