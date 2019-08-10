#! /usr/bin/env zasm
#
# Simple OS designed by Zasm
# SHA256 f04a71562e8f3947d6c9432e6df0922eb21b2f4b1ad4481436b87fbc836aeb86
#
#
# Get physical memory based on MB

define	BIOS_ENTRY_ADDR	0x7C00
define	LOADER_ADDR		0x9000

main:	@real	@vaddr:BIOS_ENTRY_ADDR
	call	&realm_get_memsize
	jmp		$

realm_print_str:	@real
	# [IN]		si: string location which is null-end
	# [OUT]
	pusha
	mov	ah	0x0E
	mov	bh	0x00
	mov	bl	0x00

	_realmode_print_str_print_:
		lodsb
		cmp		al	0x00
		jz		&_realmode_print_str_end_
		int 	0x10
		jmp		&_realmode_print_str_print_

	_realmode_print_str_end_:
		popa
		ret

realm_int2str:	@real
	# [IN]		ax: source int
	# [OUT]		si: string location with null-end
	push	ecx
	push	edi
	push	ebx
	push	edx

	xor		ecx	ecx
	mov		di	si
	mov		ebx	0x0A
	_readmode_int2str_loop_:
		xor		edx	edx
		div		ebx
		add		dx	0x30
		push	dx
		inc		ecx
		test	eax	eax
		jnz		&_readmode_int2str_loop_

	_readmode_int2str_loop_end_:
		pop		ax
		stosb
		loopz	&_readmode_int2str_loop_end_

	mov		ax	0x00
	stosb

	pop		edx
	pop		ebx
	pop		edi
	pop		ecx
	ret

realm_get_memsize:	@real
	# stage 3 - get memory by 0x88 (max to 64 MB)
	_realmode_get_memory_stage3_:
		mov		ah	0x88
		int		0x15
		jc		&_realmode_get_memsize_error_
		and		eax	0x0000FFFF
		add		eax	0x00000400
		shr		eax	0x0A

	_realmode_get_memsize_end:
		lea		si	&MEM
		call	&realm_print_str

		call	&realm_int2str
		call	&realm_print_str
		lea		si	&MBYTE
		call	&realm_print_str
		ret
	_realmode_get_memsize_error_:
		lea		si	&GET_MEMORY_FAIL
		call	&realm_print_str
		ret

data	BANNER				'ABCDEFG\x00'
data	MEM					'Mem: \x00'
data	MBYTE				' MB\x00'
data	GET_MEMORY_FAIL		'Load memory failure\x00'
block	'\x00'		$~510
block	'\x55\xAA'


