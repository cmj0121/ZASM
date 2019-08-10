/* Copyright (C) 2017-2019 cmj. All right reserved. */

#include <string>
#include <fstream>

#include "zasm.h"


void Zasm::assemble(std::string srcfile) {
	std::string line;
	std::fstream fd(srcfile);

	if (0 != this->_srclist_.count(srcfile)) {
		_D(LOG_CRIT, "duplicated file `%s`", srcfile.c_str());
		throw Exception(ERR_IO_ERROR, "duplicated source file - " + srcfile);
	} else if (!fd.is_open()) {
		_D(LOG_CRIT, "assemble file `%s` failure", srcfile.c_str());
		throw Exception(ERR_IO_ERROR, "invalid source file - " + srcfile);
	}

	this->_srclist_.insert(srcfile);
	this->_lineno_list_.push_back(this->_lineno_);
	this->_lineno_ = 0;
	while (std::getline(fd, line)) {
		Zasm::parser(line);
	}
	this->_lineno_ = this->_lineno_list_.back();
	this->_lineno_list_.pop_back();

	fd.close();
	this->_srclist_.erase(srcfile);
}

