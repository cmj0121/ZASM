/* Copyright (C) 2017-2019 cmj. All right reserved. */
#pragma once

#include "debug.h"

/* platform-specified variable */
#if  defined(__APPLE__)
#	define OFF_T	"0x%llX"
#elif defined(__linux__)
#	define OFF_T	"0x%zX"
#else
#	error "Not defined platform"
#endif /* ! __APPLE__ && ! __linux__ */

std::string escape(std::string src, bool raw_string=true);
bool asInt(std::string src, off_t &ret);
