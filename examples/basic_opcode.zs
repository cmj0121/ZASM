#! /usr/bin/env zasm
#
# check all possible opcode and machine code in ZASM
# SHA256 68a7304a672b633ece4687eeaccac6547bc1d615cfe44969d031a8e14c78d945

	nop								# 90
real_mode:	@real
	xor		ax	ax					# 31 C0
	xor		al	al					# 30 C0
	xor		ah	ah					# 30 E4

	int		0x10					# CD 10
	int		0x03					# CC
	int3							# CC

	lodsb							# AC
	lodsw							# AD
	lodsd							# 66 AD

	jmp		$						# E9 FD FF
	jmp		$$						# E9 EE FF

	jmp		0x1234					# E9 34 12
	jmp		eax						# FF E0
	jmp		0x00:0x12				# EA 12 00 00 00
	jmp		0xABCD:0x1234			# EA 34 12 CD AB
	jmp		0xAA:&real_mode			# EA DA FF AA 00

	lea		si	&real_mode			# 8D 36 01 00

entry_point_a:	@vaddr:0xAA00
	jmp		0xABCD:0x1234			# EA 34 12 CD AB
	jmp		0xAA:&entry_point_a		# EA 2D AA AA 00

prot_mode:	@protected


block	word &entry_point_a			# 2D AA		(absoulute address)
data	NOP '\x90\x90'				# 90 90
block		'\xCC\xCC'				# CC CC
block	dword 0x12ABCD34			# 34 CD AB 12
