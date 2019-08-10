/* Copyright (C) 2017-2019 cmj. All right reserved. */

#include <cstring>
#include <fstream>

#include "zasm.h"

Zasm::Zasm(std::string fmt) : _len_(0), _seclen_(0), _vaddr_(0), _arch_(ARCH_X86_LONG_MODE), _string_(true) {
	if      (fmt == "bin" || fmt == "")  this->_fmt_ = ZASM_FMT_BIN;
	else if (fmt == "elf64")             this->_fmt_ = ZASM_FMT_ELF64;
	else if (fmt == "macho64")           this->_fmt_ = ZASM_FMT_MACH_O_64;
	else {
		_D(LOG_CRIT, "Not support format %s", fmt.c_str());
		throw Exception(ERR_NOT_IMPLEMENTED, "Not support format " + fmt);
	}
}

Zasm::~Zasm() {
	for (size_t i = 0; i < this->_codes_.size(); ++i) {
		delete this->_codes_[i];
	}
}

off_t Zasm::dumpfile_bin(std::fstream &st) {
	off_t len = 0, seclen = 0;

	/* re-address the undetermined address */
	_D(LOG_DEBUG, "start re-address");
	for (auto &code : this->_codes_) {
		if ( *code ) {
			if (ZASM_CUR_POS == code->symbol()) {
				/* current instruction */
				code->set_immediate(len, seclen, len);
			} else if (ZASM_CUR_SECTION == code->symbol()) {
				/* section address     */
				code->set_immediate(len - seclen, seclen, len);
			} else {
				/* address of symbol */
				if (0 == this->_symbols_.count(code->symbol())) {
					_D(LOG_CRIT, "symbol `%s` not defined", code->symbol().c_str());
					throw Exception(ERR_SYMBOL_NOT_DEFINED, "symbol `" + code->symbol() + "` not defined");
				}
				code->set_immediate(this->_symbols_[code->symbol()], seclen, len);
			}
		}

		len += code->length();
		seclen += code->length();
		if (code->label() != "") seclen = 0x00;
	}

	len = 0;
	for (size_t i = 0; i < _codes_.size(); ++i) {
		/* dump the machine code */
		st << *_codes_[i];
		len += _codes_[i]->length();
	}

	return len;
}

off_t Zasm::dumpfile_elf64(std::fstream &st) {
	off_t entry = 0x400000;
	Elf64_Ehdr header = {};
	Elf64_Phdr segment = {0};
	Elf64_Shdr section = {0};

	st.seekg(0);

	{	/* ELF Header */
		header.e_ident[EI_MAG0]		= ELFMAG0;
		header.e_ident[EI_MAG1]		= ELFMAG1;
		header.e_ident[EI_MAG2]		= ELFMAG2;
		header.e_ident[EI_MAG3]		= ELFMAG3;
		header.e_ident[EI_CLASS]	= ELFCLASS64;
		header.e_ident[EI_DATA]		= ELFDATA2LSB;
		header.e_ident[EI_VERSION]	= EV_CURRENT;
		header.e_ident[EI_OSABI]	= ELFOSABI_SYSV;


		header.e_type		= ET_EXEC;
		header.e_machine	= EM_X86_64;
		header.e_version	= EV_CURRENT;
		header.e_entry		= entry + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr);
		header.e_phoff		= sizeof(Elf64_Ehdr);
		header.e_shoff		= 0x0;
		header.e_flags		= 0x0;
		header.e_ehsize		= sizeof(header);
		header.e_phentsize	= sizeof(Elf64_Phdr);
		header.e_phnum		= 0x01;
		header.e_shentsize	= sizeof(Elf64_Shdr);
		header.e_shnum		= 0x0;
		header.e_shstrndx	= 0x0;
	}

	{	/* ELF Segment */
		segment.p_type		= PT_LOAD;
		segment.p_flags		= PF_R | PF_X;
		segment.p_offset	= 0x0;
		segment.p_vaddr		= entry;
		segment.p_paddr		= entry;
		segment.p_filesz	= 0x0;
		segment.p_memsz		= 0x0;
		segment.p_align		= 0x1000;
	}

	st.write((char *)&header, sizeof(header));
	st.write((char *)&segment, sizeof(Elf64_Phdr));

	segment.p_filesz	= Zasm::dumpfile_bin(st);
	segment.p_memsz		= segment.p_filesz;

	/* extra information */
	if( this->_string_ ) {	/* ELF Section */
		long int off = st.tellg() % 64, shstr_size = 0;
		char buff[64] = {0};


		/* ELF Section Layout SPEC by Zerg
		 *
		 * +~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
		 * |  Binary buttom with align  |
		 * +----------------------------+
		 * |  share string table ...    |
		 * +----------------------------+
		 * |  Section 0 : dummy         |
		 * +----------------------------+
		 * |  Section 1 : shstrtab      |
		 * +----------------------------+
		 * |  Section 2 : text          |
		 * +----------------------------+
		 *
		 */

		/* align for the binary payloa */
		st.write(buff, 64 - st.tellg() % 64);

		{	/* dump all string */
			off = st.tellg();
			std::string shstrtabs[] = {"", ".shstrtabs", ".text", ""};
			for (auto key : shstrtabs) {
				st.write(key.c_str(), key.size() + 1);
			}
			shstr_size = st.tellg();
			shstr_size = shstr_size - off;
		}
		st.write(buff, 64 - st.tellg() % 64);	/* align for the binary payloa */

		/* update header */
		st.write(buff, 64 - st.tellg() % 64);	/* align for the binary payloa */
		header.e_shoff		= st.tellg();
		header.e_shentsize	= sizeof(Elf64_Shdr);
		header.e_shnum		= 3;
		header.e_shstrndx	= 1;

		/* section 0 : dummy */
		section.sh_name			= 0;
		section.sh_type			= SHT_NULL;
		section.sh_flags		= 0;
		section.sh_addr			= 0;
		section.sh_offset		= 0;
		section.sh_size			= 0;
		section.sh_link			= 0;
		section.sh_info			= 0;
		section.sh_addralign	= 0;
		section.sh_entsize		= 0;
		st.write((char *)&section, sizeof(section));

		/* section 1 : shstrtab */
		section.sh_name			= 0x01;
		section.sh_type			= SHT_STRTAB;
		section.sh_flags		= 0;
		section.sh_addr			= 0;
		section.sh_offset		= off;
		section.sh_size			= shstr_size;
		section.sh_link			= 0;
		section.sh_info			= 0;
		section.sh_addralign	= 1;
		section.sh_entsize		= 0;
		st.write((char *)&section, sizeof(section));

		/* section 2 : text */
		section.sh_name			= 0x0C;
		section.sh_type			= SHT_PROGBITS;
		section.sh_flags		= SHF_ALLOC | SHF_EXECINSTR;
		section.sh_addr			= entry;
		section.sh_offset		= header.e_entry - entry;
		section.sh_size			= segment.p_filesz;
		section.sh_link			= 0;
		section.sh_info			= 0;
		section.sh_addralign	= 0x10;
		section.sh_entsize		= 0;
		st.write((char *)&section, sizeof(section));

		st.seekp(0);
		st.write((char *)&header, sizeof(Elf64_Ehdr));
	}

	st.seekp(sizeof(header));
	st.write((char *)&segment, sizeof(Elf64_Phdr));

	return st.tellg();
}

off_t Zasm::dumpfile_macho64(std::fstream &st) {
	off_t entry = 0x1000, vmsize = 0x1000;

	/* dummy header */
	struct mach_header_64 hdr;
	st.seekg(0);
	st.write((char *)&hdr, sizeof(hdr));

	/* Segment 1 : LC_SEGMENT_64 - PAGEZERO */
	struct segment_command_64 seg_pagezero = {0x00};

	seg_pagezero.cmd		= LC_SEGMENT_64;
	seg_pagezero.cmdsize	= sizeof(seg_pagezero);
	snprintf(seg_pagezero.segname, sizeof(seg_pagezero.segname), "%s", SEG_PAGEZERO);
	seg_pagezero.vmaddr		= 0x00;
	seg_pagezero.vmsize		= vmsize;
	seg_pagezero.fileoff	= 0x00;
	seg_pagezero.filesize	= 0x00;
	seg_pagezero.maxprot	= 0x00;
	seg_pagezero.initprot	= 0x00;
	seg_pagezero.nsects		= 0x00;
	seg_pagezero.flags		= 0x00;
	st.write((char *)&seg_pagezero, sizeof(seg_pagezero));

	/* Segment 2 : LC_SEGMENT_64 - TEXT */
	struct segment_command_64 seg_text = {0x00};
	struct section_64 sec_text = {{0x00}};

	seg_text.cmd		= LC_SEGMENT_64;
	seg_text.cmdsize	= sizeof(seg_text) + sizeof(sec_text);
	snprintf(seg_text.segname, sizeof(seg_text.segname), "%s", SEG_TEXT);
	seg_text.vmaddr		= entry;
	seg_text.vmsize		= vmsize;	/* 1M - NOTE when total size > 1M need enlarge this value */
	seg_text.fileoff	= 0x00;
	seg_text.filesize	= 0x00;
	seg_text.maxprot	= VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE;
	seg_text.initprot	= VM_PROT_READ | VM_PROT_EXECUTE;
	seg_text.nsects		= 0x01;
	seg_text.flags		= 0x00;
	st.write((char *)&seg_text, sizeof(seg_text));

	/* Section 1 : SECT_TEXT */
	snprintf(sec_text.sectname, sizeof(sec_text.sectname), "%s", SECT_TEXT);
	snprintf(sec_text.segname, sizeof(sec_text.segname), "%s", SEG_TEXT);
	sec_text.addr		= 0x00;		/* memory address of this section */
	sec_text.size		= 0x00;		/* size in bytes of this section */
	sec_text.offset		= 0x00;		/* file offset of this section */
	sec_text.align		= 0x04;		/* section alignment (power of 2) */
	sec_text.reloff		= 0x00;		/* file offset of relocation entries */
	sec_text.nreloc		= 0x00;		/* number of relocation entries */
	sec_text.flags		= S_ATTR_PURE_INSTRUCTIONS | S_ATTR_SOME_INSTRUCTIONS;	/* flags (section type and attributes)*/
	sec_text.reserved1	= 0x00;													/* reserved (for offset or index) */
	sec_text.reserved2	= 0x00;													/* reserved (for count or sizeof) */
	sec_text.reserved3	= 0x00;													/* reserved */
	st.write((char *)&sec_text, sizeof(sec_text));

	/* Segment 3 : SEG_LINKEDIT */
	struct segment_command_64 seg_linkedit = {0x00};

	seg_linkedit.cmd		= LC_SEGMENT_64;
	seg_linkedit.cmdsize	= sizeof(seg_linkedit);
	snprintf(seg_linkedit.segname, sizeof(seg_linkedit.segname), "%s", SEG_LINKEDIT);
	seg_linkedit.vmaddr		= 0x00;
	seg_linkedit.vmsize		= 0x00;
	seg_linkedit.fileoff	= 0x00;
	seg_linkedit.filesize	= 0x00;
	seg_linkedit.maxprot	= VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE;
	seg_linkedit.initprot	= VM_PROT_READ;
	seg_linkedit.nsects		= 0x00;
	seg_linkedit.flags		= 0x00;
	st.write((char *)&seg_linkedit, sizeof(seg_linkedit));

	/* Segment 4 : LC_DYLD_INFO_ONLY */
	struct dyld_info_command dyld = {0x00};

	dyld.cmd		= LC_DYLD_INFO_ONLY;	/* LC_DYLD_INFO or LC_DYLD_INFO_ONLY */
	dyld.cmdsize	= sizeof(dyld);			/* sizeof(struct dyld_info_command) */
	dyld.rebase_off			= 0x00;			/* file offset to rebase info  */
	dyld.rebase_size		= 0x00;			/* size of rebase info   */
	dyld.bind_off			= 0x00;			/* file offset to binding info   */
	dyld.bind_size			= 0x00;			/* size of binding info  */
	dyld.weak_bind_off		= 0x00;			/* file offset to weak binding info   */
	dyld.weak_bind_size		= 0x00;			/* size of weak binding info  */
	dyld.lazy_bind_off		= 0x00;			/* file offset to lazy binding info */
	dyld.lazy_bind_size		= 0x00;			/* size of lazy binding infs */
	dyld.export_off			= 0x00;			/* file offset to lazy binding info */
	dyld.export_size		= 0x00;			/* size of lazy binding infs */
	st.write((char *)&dyld, sizeof(dyld));

	/* Segment 5 : LC_SYMTAB */
	struct symtab_command  symtab = {0x00};

	symtab.cmd		= LC_SYMTAB;		/* LC_SYMTAB */
	symtab.cmdsize	= sizeof(symtab);	/* sizeof(struct symtab_command) */
	symtab.symoff	= 0x00;				/* symbol table offset */
	symtab.nsyms	= 0x00;				/* number of symbol table entries */
	symtab.stroff	= 0x00;				/* string table offset */
	symtab.strsize	= 0x00;				/* string table size in bytes */
	st.write((char *)&symtab, sizeof(symtab));

	/* Segment 6 : LC_DYSYMTAB */
	struct dysymtab_command dysymtab = {0x00};

	dysymtab.cmd			= LC_DYSYMTAB;	/* LC_DYSYMTAB */
	dysymtab.cmdsize		= sizeof(dysymtab);	/* sizeof(struct dysymtab_command) */
	dysymtab.ilocalsym		= 0x00;	/* index to local symbols */
	dysymtab.nlocalsym		= 0x00;	/* number of local symbols */
	dysymtab.iextdefsym		= 0x00;/* index to externally defined symbols */
	dysymtab.nextdefsym		= 0x00;/* number of externally defined symbols */
	dysymtab.iundefsym		= 0x00;	/* index to undefined symbols */
	dysymtab.nundefsym		= 0x00;	/* number of undefined symbols */
	dysymtab.tocoff			= 0x00;	/* file offset to table of contents */
	dysymtab.ntoc			= 0x00;	/* number of entries in table of contents */
	dysymtab.modtaboff		= 0x00;	/* file offset to module table */
	dysymtab.nmodtab		= 0x00;	/* number of module table entries */
	dysymtab.extrefsymoff	= 0x00;	/* offset to referenced symbol table */
	dysymtab.nextrefsyms	= 0x00;	/* number of referenced symbol table entries */
	dysymtab.indirectsymoff	= 0x00; /* file offset to the indirect symbol table */
	dysymtab.nindirectsyms	= 0x00;  /* number of indirect symbol table entries */
	dysymtab.extreloff		= 0x00;	/* offset to external relocation entries */
	dysymtab.nextrel		= 0x00;	/* number of external relocation entries */
	dysymtab.locreloff		= 0x00;	/* offset to local relocation entries */
	dysymtab.nlocrel		= 0x00;	/* number of local relocation entries */
	st.write((char *)&dysymtab, sizeof(dysymtab));

	/* Segment 7 : LC_LOAD_DYLINKER */
	struct dylinker_command dylink = {0x00};
	#define DYDL "/usr/lib/dyld\00\00\x00\x00\x00\x00"

	memset((char *)&dylink, 0x00, sizeof(dylink));
	dylink.cmd		= LC_LOAD_DYLINKER;
	dylink.cmdsize	= sizeof(dylink) + sizeof(DYDL);	/* includes pathname string */
	dylink.name.offset	= sizeof(dylink);				/* dynamic linker's path name */
	st.write((char *)&dylink, sizeof(dylink));
	st.write(DYDL, sizeof(DYDL));

	/* Segment 8 :  LC_UNIXTHREAD*/
	uint32_t flavor, count;
	struct thread_command lc_unixthread;
	x86_thread_state64_t thread_state = {0x00};

	lc_unixthread.cmd		= LC_UNIXTHREAD;
	lc_unixthread.cmdsize	= sizeof(lc_unixthread) + sizeof(thread_state) + 2 * sizeof(uint32_t);
	flavor					= x86_THREAD_STATE64;
	count					= x86_THREAD_STATE64_COUNT;
	st.write((char *)&lc_unixthread, sizeof(lc_unixthread));
	st.write((char *)&flavor, sizeof(uint32_t));
	st.write((char *)&count, sizeof(uint32_t));
	off_t offset_unixthread = st.tellg();
	st.write((char *)&thread_state, sizeof(thread_state));

	off_t offset = st.tellg();
	/* dump the machine code */
	off_t bin_size = Zasm::dumpfile_bin(st);

	/* HACK - The mach-o 64 minimal file size: 4K */
	while (0x1000 > st.tellg()) {
		st.write("\x00", 1);
	}

	/* HACK - Re-fill the header */
	off_t total_filesize = st.tellg();
	/* Mach-O 64 header */
	st.seekg(0);
	hdr.magic 		= MH_MAGIC_64;										/* mach magic number identifier */
	hdr.cputype		= CPU_TYPE_X86_64;									/* CPU specifier */
	hdr.cpusubtype	= CPU_SUBTYPE_LIB64 | CPU_SUBTYPE_VAX750;			/* machine specifier */
	hdr.filetype	= MH_EXECUTE;										/* type of file */
	hdr.ncmds		= 0x08;												/* number of load commands */
	hdr.sizeofcmds	= offset - sizeof(hdr);								/* the size of all the load commands */
	hdr.flags		= MH_NOUNDEFS | MH_DYLDLINK | MH_TWOLEVEL | MH_PIE;	/* flags */
	hdr.reserved	= 0x00;												/* reserved */
	st.write((char *)&hdr, sizeof(hdr));

	/* Segment 2 : LC_SEGMENT_64 - TEXT */
	st.seekg(sizeof(hdr) + sizeof(seg_text));
	seg_text.filesize	= bin_size + offset;
	st.write((char *)&seg_text, sizeof(seg_text));
	/* Section 1 : SECT_TEXT */
	sec_text.addr		= entry + offset;	/* memory address of this section */
	sec_text.size		= bin_size;			/* size in bytes of this section */
	sec_text.offset		= offset;			/* file offset of this section */
	st.write((char *)&sec_text, sizeof(sec_text));
	/* Segment 3 : SEG_LINKEDIT */
	seg_linkedit.fileoff= total_filesize;   /* Don't know why need large than 4K */
	st.write((char *)&seg_linkedit, sizeof(seg_linkedit));
	/* Segment 8 : LC_UNIXTHREAD */
	st.seekp(offset_unixthread);
	thread_state.__rip = entry + offset;
	st.write((char *)&thread_state, sizeof(thread_state));

	return st.tellg();
}


Zasm& operator+= (Zasm& zasm, ZasmCode *code) {
	BUG( NULL == code);
	zasm._codes_.push_back(code);
	if (code->label() != "") {
		if (0 != zasm._symbols_.count(code->label())) {
			_D(LOG_CRIT, "multiple symbol declaim - `%s`", code->label().c_str());
			throw Exception(ERR_SYNTAX_ERROR, "multiple symbol declaim - `" + code->label() + "`");
		}

		zasm._symbols_[ code->label() ] = zasm._len_;
		_D(LOG_INFO, "add new symbol `%s` %lX", code->label().c_str(), zasm._len_);
	}

	zasm._len_ += code->length();
	zasm._seclen_ = (0 == code->length()) ? 0 : (zasm._seclen_ + code->length());

	return zasm;
}

std::fstream& operator << (std::fstream &st, Zasm &src) {
	switch (src._fmt_) {
		case ZASM_FMT_BIN:
			src.dumpfile_bin(st);
			break;
		case ZASM_FMT_ELF64:
			src.dumpfile_elf64(st);
			break;
		case ZASM_FMT_MACH_O_64:
			src.dumpfile_macho64(st);
			break;
		default:
			throw Exception(ERR_NOT_IMPLEMENTED, "Not implemented the format");
			break;
	}

	return st;
}

