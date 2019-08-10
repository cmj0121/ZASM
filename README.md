# ZASM #
This is the note for the ZASM development. ZASM is the assembly language used on Zerg.
Now Zasm support following parameters when compile source code

- v, verbose for verbose message
- f, format for specified platform of the final binary file
- o, output for specified output file name

- [Step 1.1 - Syntax Design](#step-1-1)
- [Step 1.2 - Lexer](#step-1-2)
- [Step 1.3 - Parser](#step-1-3)
- [Step 1.4 - Emit Machine Code](#step-1-4)
- [Step 1.5 - Re-address](#step-1-5)
- [Step 1.6 - Decorator Process](#step-1-6)


## Step 1-1 ##
Before implement the ZASM I need to design the ZASM grammar and it would be as simple as
it could. In ZASM there are six grammar and few reserved symbol, like as following:

```grammar
stmt      : ( code | label | define | data | block | include )? [ COMMENT ] NEWLINE
code      : OPCODE    ( operand )*
label     : TOKEN     ':' ( decorator )*
include   : 'include' STRING
define    : 'define'  TOKEN  ( IMM | STRING )
block     : 'block'   buffer [ range ]
data      : 'data'    TOKEN  buffer

operand   : REG | MEM | IMM | reference
decorator : '@' TOKEN [ ':' TOKEN ]
reference : IMM | '&' TOKEN | '$' | '$$'
buffer    : STRING | SIZE reference
range     : IMM | ( ( IMM | reference ) '~' ( IMM | reference ) )
```

In ZASM each line would be contained a complete [statement][0], comment or empty line, and
could be assembled into one specified [machine code][1]. This process is call *assemble*.
The coding style in ZASM is *opcode destination source* style which like the [Intel Syntax][2]

- opcode destination source
- simple register words, like **rax**, **eip**
- general digital, binary (0b-prefix), octal (0o-prefix) and hexadecimal (0x-prefix)
- reserved **byte**, **word**, **dword** and **qword**
- memory address via [ segment register : base register + index register * scale + immediate]

### Reserved Word ###
ZASM also provides several convenient reserved words for develop quick and smoothly.
It is a [syntax sugar][3] and reduce the repeat works when design new program via ZASM.

The first reserved word is *include* which is used to include the external ZASM source file
without repeatedly write the same code. ZASM will assemble the another source code when detect
the include statement, which means the ZASM will replace the full contains of the external file
with this line.


The second used reserved word is *define* which likes the include but just replace the token.
ZASM will replace the token when ZASM detect this statement contains a token already defined
in previous statements. The *define* statement is used when need to define a global variable.


The *data* and *block* are similar statements with just few difference. The *data* is used to
define a raw memory contains with naming, and the *block* statement is used to define a data
without naming. When data statement is created, ZASM will keep the data and write into the
[data segment][4] if possible. But the *block* statement ZASM will write the data as a raw
machine code. With extra parameter the block statement can write more than one time.


### Reference Symbol ###
ZASM provides several useful syntax for the reference address, like *$* means the current
machine code and *$$* is used for current section position. In the label statement we can
define a pseudo machine code with label, and other code flow can jump or use via specified
reference symbol *&*.

### Decorator Symbol ###
ZASM may not provide the full functionality for each platform and purpose. The decorator
symbol *@* is used for these cases. When developer need give some hint or extra functionality
on ZASM, he/her can use the decorator with extra parameter. Like **@real** is used to give
the hint for [real mode][5]


## Step 1-2 ##
The lexer in ZASM is the first part for generating the final machine code. The `Zasm::lexer` function
can separate the source code into several valid tokens. In general the valid tokens, like NEWLINE,
IMM, STRING, MEMORY, TOKEN and RESERVED and all are used on `Zasm::parser`.


The ZASM will reply the current line in lexer stage, the line buffer is saved via the `Zasm::parser`
and reply the valid token. The token NEWLINE means there are no more valid token in ZASM statement,
and STRING is a raw memory buffer which may contains escape characters. IMM is the digit number or
other kind of binary, octal and/or hexadecimal. MEMORY is a specified memory address used on the
ZASM.

## Step 1-3 ##
The ZASM supports six statements and would generate the pseudo code in `ZasmCode` structure. The major
statement used in ZASM is the code statement: these statements are assembled to related and specified
machine code and machine code also can be reverted to one and only code statement. The code statement
contains the only one opcode and at most two operand in this version ZASM. The label statement is a
special statement which is used to separate ZASM code statements into several logical blocks. In
ZASM both of two are used the `code_label_statement` to parse the statement generate the final machine
code.


In parse stage, ZASM first detect the first token and pass to next parse stage. Choosing one of six
possible tier-two parser and run the remainder parser flow. In some statements, like label, block and
data need another expression parser. Within the parser stage ZASM also process the decorator and save
template machine code in memory. It will be described in Step 1-4.


## Step 1-4 ##
In this stage ZASM starts to generate the machine code. Without of the general, ZASM generate machine
code only, which is called binary format, and could generate other platform binary like [Mach-O][6]
and [ELF][7]. In state 3 ZASM parse the source code and only three statements are directly related
to machine code: code statement, block statement and data block statement. Other statements are
used for reference and re-address the existed machine code.


The `ZasmCode` is used to generate the machine code. It contains the raw [unsigned char][8] buffer
and other public method that can use to generate the machine code.

In the emit staging ZASM could be generate the final binary for each platform, like raw binary,
ELF 64 and Macho-O 64, which is depended on parameter pass to Zasm. Without of the general,
Zasm could generate on each platform event not be compiled on related platform. Programmers could
specified the platform via the `-f` parameter when compile the source code.

### Operand Detection ###
In each architecture the operand could be anything. In x86 family the operand can be a register,
memory or immediate number. The inner class `Operand` is used to help and analysis the raw string
and get the related meta information for each operand. For example, the memory can be separate into
the size, basic register and offset.

### Machine Code Emit ###
The ZASM support x86 machine codes emit via the `ZasmCode::emit` function and implemented in
`src/zasm/architecture/x86_64.cpp`. In x86 family the machine code is no more than 16 bits and can
be separated into five fields: prefix, opcode, mod R/W, displacement and immediate. The prefix is
at most four bytes of machine code decorator, it can be described the size and/or extra segment register.
The opcode is the main field and each instruction has its own opcode. And more, opcode can has extra
`0x0F` prefix and means the another opcode. The mod R/W field and extra SIB is the field that
help to describe the size of memory operand, like the scale and the shift of memory.


The remainder two filed is used to shift via immediate. The displacement field is used to describe
the immediate position of the memory and the immediate is used on immediate in machine code directly.

### Emit Memory ###
When emit the x86-family machine code, the memory-part is the annoying part: it contains so many detail
and could be format in other parts. In x86-family the memory contains the size, the segment offset, the
based register, the scale and index register and displacement of the memory. It may contains the based
register or only displacement, like a directly memory access. Each part would be described by the Mod R/M,
one byte to describe the type of memory. And more, the SIB byte is used to describe the extra format of
the memory, like the scale and which based and/or index register used.

### Emit Displacement ###
This part of machine code is used to describe the displacement of the memory. Without of the generate
this part should be within 1 byte to 4 bytes. But in some extreme case the displacement could be 8-bytes
long.


### Emit Immediate ###
In the last x86-family machine code field, the immediate field is used to describe the number of
absolute integer in machine code. It can be the digit or the memory address. The `ZasmCode::emit`
implements in the last block and detect the operands is the immediate or the reference. In case of
pure immediate number, ZASM generate the related size of immediate number and update the machine code
via the inner function *set_immediate*.


## Step 1-5 ##
In the ZASM there are three cases of instructions that need to be re-address: `$` for currently
address, `$$` for the current block section and block `&` used for the referenced token. All of
three cases are be done in the *Zasm::dumpfile_bin* method. Before dump the machine code, ZASM
will check the pseudo machine code need to be re-address or not. When this pseudo code need to
be re-address the ZASM first check the symbol and calculate the value need to use. Calling the
function `set_immediate` with extra reset parameter the ZASM will overwrite the undetermined
value, usually the immediate number, and set the code determined.


When calculating the new address, the ZASM need decide the new address is absolute or related.
In case of related address, the new address is calculated via the symbol absolute location
of symbol and more subtract the current length of machine code. In the case of absolute symbol,
the ZASM need calculated the symbol of address and extra based virtual address.

### reference address ###
The ZASM supports `$` and `$$` which are used to related address, like current address and
current section address. In ZASM the parse time, it would translate to the related address
and set as related immediate number.


## Step 1-6 ##
The extra part of ZASM is the decorator: it is used to give extra information about the
next machine code, like the architecture, virtual address and other. The decorators are
transitive and would be overridden with new decorator,


[0]: https://en.wikipedia.org/wiki/Statement_(computer_science)
[1]: https://en.wikipedia.org/wiki/Machine_code
[2]: https://en.wikipedia.org/wiki/X86_assembly_language#Syntax
[3]: https://en.wikipedia.org/wiki/Syntactic_sugar
[4]: https://en.wikipedia.org/wiki/Data_segment
[5]: https://en.wikipedia.org/wiki/Real_mode
[6]: https://en.wikipedia.org/wiki/Mach-O
[7]: https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
[8]: https://en.wikipedia.org/wiki/C_data_types#Basic_types

