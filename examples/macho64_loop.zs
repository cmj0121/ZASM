#! /usr/bin/env zasm
#
# Simple loop in Mach-O 64 environment
# SHA256 584dd56f581aa7052cf394a8625b010af9339e77e62004f985aeaae9d5b62230

define	SYSCALL_EXIT	0x2000001
define	SYSCALL_WRITE	0x2000004

__main__:
	mov		r9	0x10
	call	&fn

	mov		rax	SYSCALL_EXIT
	syscall
fn:
	mov		rax	SYSCALL_WRITE
	mov		rdi	0x01			# STDOUT
	lea		rsi	&msg			# message
	mov		rdx 0x0E			# length
	syscall

	dec		r9
	cmp		r9	0x00
	jne		&fn
	ret

data	msg	"Countdown ...\n"
