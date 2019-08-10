#! /usr/bin/env zasm
#
# Simple OS designed by Zasm
# SHA256 1f9b4d804dad5285d5ddfe30cd1ddb8faab76e90b9fa04b0d02f25f55a585bb1
#
#
# Physical Memory in BIOS
#
# <  1 MB
#	0000 0000 		real-mode IVT (Interrupt vector Table)
#	0000 0400		BDA
#	0000 0500		BDA (BIOS Data Area)
#	0000 7C00		OS boot sector
#	0000 7E00		Conventional Memory
#	0008 0000		Conventional Memory
#	0009 FC00		EBDA (extended BIOS Data Area)
#	000A 0000		Video memory / ROM

define	BIOS_ENTRY_ADDR	0x7C00
define	LOADER_ADDR		0x9000

__boot_loader__:	@real	@vaddr:BIOS_ENTRY_ADDR
	mov	ax	BIOS_ENTRY_ADDR

	mov	ax	0x0600	# clean-up screen
	mov	bx	0x0700
	mov	cx	0x00
	mov	dx	0x184F	# (80, 25)
	int 0x10		# interrupt

	call	&_read_disk_via_chs_

	mov	ax	LOADER_ADDR
	jmp	ax	# CS:AX	0x0000:LOADER_ADDR

_read_disk_via_chs_:
	mov	ax	0x00
	mov	es	ax

	mov	ah	0x02		# read sector from driver
	mov	al	0x01		# number of sector to read
	mov	ch	0x00		# # of cylinder
	mov cl	0x02		# # of sector
	mov	dh	0x00		# # of head
	mov	dl	0x00		# # of drive
	mov	bx	LOADER_ADDR	# memory address data write to ES:BX
	int		0x13

	jc		&_retry_read_disk_

	mov	ah	0x0E	# teletype Output
	mov	al	0x2D	# char '-'
	mov	bh	0x00	# page Number
	mov	bl	0x00	# color
	int 0x10		# interrupt

	ret

_retry_read_disk_:
	xor		ah	ah	# reset drive
	int		0x13

	mov	ah	0x0E	# teletype Output
	mov	al	0x2E	# char '.'
	mov	bh	0x00	# page Number
	mov	bl	0x00	# color
	int 0x10		# interrupt
	jmp	&_read_disk_via_chs_

block	'\x00'		$~510
block	'\x55\xAA'

_loader_:
	mov	ah	0x0E	# teletype Output
	mov	al	0x40	# char '@'
	mov	bh	0x00	# page Number
	mov	bl	0x00	# color
	int 0x10		# interrupt

	hlt				# Halt
	jmp	$

