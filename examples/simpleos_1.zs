#! /usr/bin/env zasm
#
# Simple OS (boot-loader) on Zasm
# SHA256 faacc83a6475d446d766c6b53b3da73b24888d5f5a10292effcb40e996845c53

__boot_loader__:	@real	@vaddr:0x7C00
	lea	si	&BANNER			# 8D 36 19 7C

	mov	ah	0x0E			# B4 0E
	mov	bh	0x00			# B7 00
	mov	bl	0x00			# B3 00

_print_:
	lodsb					# AC
	cmp		al	0x0			# 3C 00
	jz		&loop			# 74 05
	int 	0x10			# CD 10
	jmp		&_print_		# E9 F6 FF

loop:
	jmp		$				# E9 FD FF

data	BANNER		'Simple OS (boot loader) on Zasm'
block	'\x00'		$~510
block	'\x55\xAA'
