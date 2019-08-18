#! /usr/bin/env zasm
#
# Calculate the length of string
# SHA256 f968c1eac9051c1f20ae44963e5825d54b2d229c20a42e7b4cffe8e39943a1d5

define	SYSCALL_EXIT	0x2000001
define	SYSCALL_WRITE	0x2000004

__main__:
	lea		rsi	&msg
	call	&_strlen_

	mov		rax	SYSCALL_WRITE
	mov		rdi	0x01				# STDOUT
	lea		rsi	&msg				# message
	mov		rdx rcx					# length
	syscall

	mov		rax	SYSCALL_EXIT
	syscall

_strlen_:
	# INPUT  : RDI (string)
	# OUTPUT : RCS (length of string)
	xor		rcx	rcx
	_strlen_inner_:
		lodsb
		test	al	al
		jz		&_strlen_end_
		inc		rcx
		jmp		&_strlen_inner_
	_strlen_end_:
	ret

data	msg	"Hello World\n"
