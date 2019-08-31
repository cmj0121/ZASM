#! /usr/bin/env sh
#
# Simple function from string to integer (ATOI)
# SHASUM 41d97f8b0d4193b4b9a1b849ef24d03fd6270e24cbdb36aef3028c19ffe4e8ad

define	SYSCALL_EXIT	60
define	SYSCALL_WRITE	1

__main__:
	mov		r8		[rsp]				# ARGC

	cmp		r8		0x01
	jz		&failure

	mov		rsi		[rsp + 0x10]		# ARGV[1]
	call	&atoi
	mov		rdi	rcx
	call	&__exit__

atoi:
	push	rsi
	xor		rcx	rcx

	_atoi_inner_:
		lodsb
		test	al	al
		jz		&_atoi_end_

		cmp		al	0x30
		jl		&failure
		cmp		al	0x3A
		jge		&failure

		sub		al	0x30
		imul	ecx	0x0A
		add		rcx	rax
		jmp		&_atoi_inner_

	_atoi_end_:
		pop		rsi
		ret

failure:
	lea		rsi	&ERROR_MSG
	call	&__print_str__
	jmp		&__exit__

__exit__:
	mov		rax	SYSCALL_EXIT
	syscall

__print_str__:
	push	rsi
	call	&__strlen__

	mov		rax	SYSCALL_WRITE
	mov		rdi	0x01				# STDOUT
	pop		rsi
	mov		rdx rcx					# length
	syscall

	ret

__strlen__:
	# INPUT  : RSI (string)
	# OUTPUT : RCX (length of string)
	xor		rcx	rcx
	_strlen_inner_:
		lodsb
		test	al	al
		jz		&_strlen_end_
		inc		rcx
		jmp		&_strlen_inner_
	_strlen_end_:
	ret

data	ERROR_MSG	"Need a digit\n\0"
