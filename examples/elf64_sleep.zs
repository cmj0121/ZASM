#! /usr/bin/env sh
#
# call nanosleep in Linux system
# SHA256 a416f48118e1b2fa46e60d871e5a0d396ac16505e0924305f622aa66f5998a30

define	SYSCALL_EXIT	60
define	SYSCALL_NANO_SLEEP	35

main:
	mov	eax	SYSCALL_NANO_SLEEP
	lea	rdi	&timestamp
	xor	rsi	rsi
	syscall

	mov		eax	SYSCALL_EXIT
	syscall

	timestamp:
		block	qword 0x01			# seconds
		block	qword 0x12345678	# nano seconds
