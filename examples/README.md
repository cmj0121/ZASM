# Example Code #
There are many ZASM example code which may 1) run as the small OS, 2)
run in the Linux-platofrm or 3) run in the Mac OS platform.


In the Linux-platform, you can debug via the [gdb][0] as following

	:::
	> info file		# show the entry address
	> b *0x400078	# set break point in entry point
	> r				# run the program and will stop at break point
	> nexti			# execute the next instruction (machine code)


[0]: https://en.wikipedia.org/wiki/GNU_Debugger

