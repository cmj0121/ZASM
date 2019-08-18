#! /usr/bin/env zasm
#
# Reply the argument
# SHA256 a92d41750a351ae217417fc2bae78637fd4b9d920d6e5eaa80be47c1e57a6af9

define	SYSCALL_EXIT	0x2000001
define	SYSCALL_WRITE	0x2000004

__main__:
	mov		rcx	[rsp]					# ARGC

	cmp		rcx	0x01
	jne		&next

	mov		rax	SYSCALL_WRITE
	mov		rdi	0x01					# STDOUT
	lea		rsi	&msg1					# message
	mov		rdx 0x0D					# length
	syscall
	jmp		&exit

	next:
	mov		rax	SYSCALL_WRITE
	mov		rdi	0x01					# STDOUT
	lea		rsi	&msg2					# message
	mov		rdx 0x0A					# length
	syscall

	exit:
	mov		rax	SYSCALL_EXIT
	syscall

data	msg1	"Without ARGV\n"
data	msg2	"With ARGV\n"
