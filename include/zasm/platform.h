
#ifdef __APPLE__
#	include <mach-o/loader.h>

#define EI_NIDENT		16

#define EI_MAG0			0
#define EI_MAG1			1
#define EI_MAG2			2
#define EI_MAG3			3
#define EI_CLASS		4
#define EI_DATA			5
#define EI_VERSION		6
#define EI_OSABI		7
#define EI_ABIVERSION	8

#define ELFMAG0			0x7f
#define	ELFMAG1			'E'
#define	ELFMAG2			'L'
#define	ELFMAG3			'E'
#define	ELFCLASS64		2
#define	ELFDATA2LSB		1
#define	EV_CURRENT		1
#define	ELFOSABI_SYSV	0
#define SHT_NULL		0
#define	SHT_PROGBITS	1
#define	SHT_STRTAB		3
#define SHF_ALLOC		(1 << 1)
#define SHF_EXECINSTR	(1 << 2)
#define ET_EXEC			2
#define EM_X86_64		62
#define PT_LOAD			1
#define PF_X 			(1 << 0)
#define PF_W 			(1 << 1)
#define PF_R 			(1 << 2)

typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef	int32_t  Elf64_Sword;
typedef uint64_t Elf64_Xword;
typedef	int64_t  Elf64_Sxword;
typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef uint16_t Elf64_Section;
typedef Elf64_Half Elf64_Versym;

typedef struct {
	unsigned char	e_ident[EI_NIDENT];
	Elf64_Half	e_type;
	Elf64_Half	e_machine;
	Elf64_Word	e_version;
	Elf64_Addr	e_entry;
	Elf64_Off	e_phoff;
	Elf64_Off	e_shoff;
	Elf64_Word	e_flags;
	Elf64_Half	e_ehsize;
	Elf64_Half	e_phentsize;
	Elf64_Half	e_phnum;
	Elf64_Half	e_shentsize;
	Elf64_Half	e_shnum;
	Elf64_Half	e_shstrndx;
} Elf64_Ehdr;
typedef struct {
	Elf64_Word	p_type;
	Elf64_Word	p_flags;
	Elf64_Off	p_offset;
	Elf64_Addr	p_vaddr;
	Elf64_Addr	p_paddr;
	Elf64_Xword	p_filesz;
	Elf64_Xword	p_memsz;
	Elf64_Xword	p_align;
} Elf64_Phdr;
typedef struct {
	Elf64_Word	sh_name;
	Elf64_Word	sh_type;
	Elf64_Xword	sh_flags;
	Elf64_Addr	sh_addr;
	Elf64_Off	sh_offset;
	Elf64_Xword	sh_size;
	Elf64_Word	sh_link;
	Elf64_Word	sh_info;
	Elf64_Xword	sh_addralign;
	Elf64_Xword	sh_entsize;
} Elf64_Shdr;
#elif __linux__

#	include <elf.h>

typedef int	cpu_type_t;
typedef int cpu_subtype_t;
typedef int	vm_prot_t;

#define MH_MAGIC_64		0xfeedfacf			/* the 64-bit mach magic number */
#define	MH_EXECUTE		0x2					/* demand paged executable file */
#define VM_PROT_READ	((vm_prot_t) 0x01)	/* read permission */
#define VM_PROT_WRITE	((vm_prot_t) 0x02)	/* write permission */
#define VM_PROT_EXECUTE	((vm_prot_t) 0x04)	/* execute permission */

#define CPU_TYPE_X86		((cpu_type_t) 7)
#define CPU_ARCH_ABI64		0x01000000	/* 64 bit ABI */
#define	CPU_TYPE_X86_64		(CPU_TYPE_X86 | CPU_ARCH_ABI64)

#define CPU_SUBTYPE_LIB64	0x80000000	/* 64 bit libraries */
#define CPU_SUBTYPE_VAX750	((cpu_subtype_t) 3)

#define LC_REQ_DYLD			0x80000000
#define	LC_SEGMENT			0x1		/* segment of this file to be mapped */
#define	LC_SYMTAB			0x2		/* link-edit stab symbol table info */
#define	LC_UNIXTHREAD		0x5		/* unix thread (includes a stack) */
#define	LC_DYSYMTAB			0xb		/* dynamic link-edit symbol table info */
#define	LC_LOAD_DYLIB		0xc		/* load a dynamically linked shared library */
#define	LC_ID_DYLIB			0xd		/* dynamically linked shared lib ident */
#define LC_LOAD_DYLINKER	0xe		/* load a dynamic linker */
#define	LC_SEGMENT_64		0x19	/* 64-bit segment of this file to be mapped */
#define	LC_DYLD_INFO_ONLY (0x22|LC_REQ_DYLD)	/* compressed dyld information only */

#define S_ATTR_PURE_INSTRUCTIONS 0x80000000	/* section contains only true machine instructions */
#define S_ATTR_SOME_INSTRUCTIONS 0x00000400	/* section contains some machine instructions */

#define	SEG_PAGEZERO	"__PAGEZERO"	/* the pagezero segment which has no */
#define	SEG_TEXT		"__TEXT"		/* the tradition UNIX text segment */
#define	SECT_TEXT		"__text"		/* the real text part of the text */
#define	SEG_LINKEDIT	"__LINKEDIT"	/* the segment containing all structs */

#define	MH_NOUNDEFS	0x1
#define MH_DYLDLINK	0x4
#define MH_TWOLEVEL	0x80
#define	MH_PIE 		0x200000

#define x86_THREAD_STATE64			4
#define x86_THREAD_STATE64_COUNT	42


union lc_str {
	uint32_t	offset;
	char		*ptr;
};
struct mach_header_64 {
	uint32_t magic;
	cpu_type_t cputype;
	cpu_subtype_t cpusubtype;
	uint32_t filetype;
	uint32_t ncmds;
	uint32_t sizeofcmds;
	uint32_t flags;
	uint32_t reserved;
};
struct segment_command_64 {
	uint32_t	cmd;
	uint32_t	cmdsize;
	char		segname[16];
	uint64_t	vmaddr;
	uint64_t	vmsize;
	uint64_t	fileoff;
	uint64_t	filesize;
	vm_prot_t	maxprot;
	vm_prot_t	initprot;
	uint32_t	nsects;
	uint32_t	flags;
};
struct section_64 {
	char		sectname[16];
	char		segname[16];
	uint64_t	addr;
	uint64_t	size;
	uint32_t	offset;
	uint32_t	align;
	uint32_t	reloff;
	uint32_t	nreloc;
	uint32_t	flags;
	uint32_t	reserved1;
	uint32_t	reserved2;
	uint32_t	reserved3;
};
struct dyld_info_command {
	uint32_t   cmd;
	uint32_t   cmdsize;
	uint32_t   rebase_off;
	uint32_t   rebase_size;
	uint32_t   bind_off;
	uint32_t   bind_size;
	uint32_t   weak_bind_off;
	uint32_t   weak_bind_size;
	uint32_t   lazy_bind_off;
	uint32_t   lazy_bind_size;
	uint32_t   export_off;
	uint32_t   export_size;
};
struct symtab_command {
	uint32_t	cmd;
	uint32_t	cmdsize;
	uint32_t	symoff;
	uint32_t	nsyms;
	uint32_t	stroff;
	uint32_t	strsize;
};
struct dysymtab_command {
    uint32_t cmd;
    uint32_t cmdsize;
    uint32_t ilocalsym;
    uint32_t nlocalsym;
    uint32_t iextdefsym;
    uint32_t nextdefsym;
    uint32_t iundefsym;
    uint32_t nundefsym;
    uint32_t tocoff;
    uint32_t ntoc;
    uint32_t modtaboff;
    uint32_t nmodtab;
    uint32_t extrefsymoff;
    uint32_t nextrefsyms;
    uint32_t indirectsymoff;
    uint32_t nindirectsyms;
    uint32_t extreloff;
    uint32_t nextrel;
    uint32_t locreloff;
    uint32_t nlocrel;
};
struct dylinker_command {
	uint32_t cmd;
	uint32_t cmdsize;
	union lc_str name;
};
struct thread_command {
	uint32_t cmd;
	uint32_t cmdsize;
};
struct x86_thread_state64_t {
	uint64_t __rax;
	uint64_t __rbx;
	uint64_t __rcx;
	uint64_t __rdx;
	uint64_t __rdi;
	uint64_t __rsi;
	uint64_t __rbp;
	uint64_t __rsp;
	uint64_t __r8;
	uint64_t __r9;
	uint64_t __r10;
	uint64_t __r11;
	uint64_t __r12;
	uint64_t __r13;
	uint64_t __r14;
	uint64_t __r15;
	uint64_t __rip;
	uint64_t __rflags;
	uint64_t __cs;
	uint64_t __fs;
	uint64_t __gs;
};
#endif

