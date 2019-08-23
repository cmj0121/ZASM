#! /usr/bin/env sh
#
# Show all the ARGV and ENVIRONMENT
# SHA256 4ad05c8b9987c6b8b65c7aa9d432990665a58b25e3d3924385affe9767ed29bf

define	SYSCALL_EXIT	60
define	SYSCALL_WRITE	1

__main__:
	# The STACK for the Linux system when process execute
	#
	# rsp	+----------------------+
	#		|    ARGC              |
	#		+----------------------+
	#		|    ARGV[0]           |
	#		|----------------------|
	#		|    ARGV[1]           |
	#		|----------------------|
	#		~                      ~
	#		|----------------------|
	#		|    ARGV[ARGV - 1]    |
	#		+----------------------|
	#       |    NULL              |
	#		+----------------------+
	#		|    ENV[0]            |
	#		|----------------------|
	#		|    ENV[1]            |
	#		|----------------------|
	#		~                      ~
	#		|----------------------|
	#		|    ENV[n]            |
	#		+----------------------|
	#       |    NULL              |

	mov		r8		[rsp]				# ARGC

	# dump all ARGV
	xor		r9		r9
	xor		r10		r10

	lea		rsi		&ARGV
	call	&__print_str__
	_loop_argv_:
		add		r10		0x08
		mov		rsi		[rsp + r10]
		call	&__print_str__

		lea		rsi		&NEWLINE
		call	&__print_str__

		inc		r9
		cmp		r8	r9
		jnz		&_loop_argv_

	add		r10		0x08
	lea		rsi		&NEWLINE
	call	&__print_str__
	lea		rsi		&ENV
	call	&__print_str__
	_loop_env_:
		add		r10		0x08
		mov		rsi		[rsp + r10]

		cmp		rsi		0x00
		jz		&__exit__

		call	&__print_str__

		lea		rsi		&NEWLINE
		call	&__print_str__

		jmp		&_loop_env_

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
	# INPUT  : RDI (string)
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

data	ARGV	"ARGV\n\0"
data	ENV		"ENV\n\0"
data	NEWLINE	"\n\0"
