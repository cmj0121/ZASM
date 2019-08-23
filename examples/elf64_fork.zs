#! /usr/bin/env sh
#
# Linux-based fork syscall
# SHA256 430cc661e4a50e23ded8b2266f8e81c8b8d9c974cf09e1abeb8997fd3b2e957a


define	SYSCALL_EXIT	60
define	SYSCALL_WRITE	1
define	SYSCALL_FORK	57

__main__:
	mov		rax	SYSCALL_FORK
	syscall

	cmp	rax	0x00
	jz	&_child_proc_	# RAX is the PID and child process got 0

	_parent_proc_:
		lea		rsi	&MSG_PARENT
		call	&_print_str_
		jmp		&_end_

	_child_proc_:
		lea		rsi	&MSG_CHILD
		call	&_print_str_
		jmp		&_end_

	_end_:
		mov		rax	SYSCALL_EXIT
		syscall

_print_str_:
	# RSI is the string
	push	rsi
	call	&_strlen_

	mov		rax	SYSCALL_WRITE
	mov		rdi	0x01				# STDOUT
	pop		rsi
	mov		rdx rcx					# length
	syscall

	ret

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

data	MSG_PARENT	"I'm parent process\n\0"
data	MSG_CHILD	"I'm child process\n\0"
