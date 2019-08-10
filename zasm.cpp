/* Copyright (C) 2017-2019 cmj. All right reserved. */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "zasm.h"

int  __verbose__ = 0;

void help(void) {
	fprintf(stderr, "ZASM (v%d.%d) - Zerg assembler\n", ZASM_VER_MAJOR, ZASM_VER_MINOR);
	fprintf(stderr, "\n");
	fprintf(stderr, "Usage - zasm [option] src\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "    -v, --verbose          Show the verbose message\n");
	fprintf(stderr, "    -f, --format           Format of the output file \n");
	fprintf(stderr, "    -o, --output <file>    Output file to <file>\n");
	exit(-1);
}
int main(int argc, char *argv[]) {
	int optIdx = -1;
	char ch, opts[] = "hvf:o:";
	struct option options[] = {
		{"help",	no_argument,		0, 'h'},
		{"verbose",	optional_argument,	0, 'v'},
		{"format",	required_argument,	0, 'f'},
		{"output",	required_argument,	0, 'o'},
		{NULL, 0, 0, 0}
	};
	std::string output = ZASM_DEFAULT_OUTPUT, fmt = "";

	while (-1 != (ch = getopt_long(argc, argv, opts, options, &optIdx))) {
		switch(ch) {
			case 'h':
				help();
				break;
			case 'v':
				__verbose__++;
				break;
			case 'f':
				fmt = optarg;
				break;
			case 'o':
				output = optarg;
				break;
		}
	}
	argc -= optind;
	argv += optind;

	if ( argc != 1 ) help();

	Zasm *zasm = new Zasm(fmt);
	try {
		zasm->assemble(argv[0]);

		std::fstream file(output, std::ios::out | std::ios::trunc);
		chmod(output.c_str(), 0755);
		file << *zasm;
	} catch (Exception &e) {
		switch ( e.code() ) {
			case ERR_IO_ERROR:
				_D(LOG_CRIT, "file operation failure on L#%zu - %s", zasm->lineno(), e.what());
				break;
			case ERR_SYNTAX_ERROR:
				_D(LOG_CRIT, "syntax error on L#%zu - %s", zasm->lineno(), e.what());
				break;
			case ERR_INVALID_TOKEN:
				_D(LOG_CRIT, "invalid token on L#%zu - %s", zasm->lineno(), e.what());
				break;
			default:
				_D(LOG_CRIT, "unknown exception #%d on L#%zu - %s", e.code(), zasm->lineno(), e.what());
				break;
		}

		return -1;
	}
	delete zasm;

	return 0;
}
/* vim set: tabstop=4 */
