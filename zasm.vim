" ZASM Syntax File
" Language:      ZASM
" Maintainer:    cmj <cmj@cmj.tw>
" Last Change:   2017 Dec 01
"
" ZASM Grammar
"
"    stmt      : ( code | label | define | data | block | include )? [ COMMENT ] NEWLINE
"
"    code      : OPCODE    ( operand )*
"    label     : TOKEN     ':' ( decorator )*
"    include   : 'include' STRING
"    define    : 'define'  TOKEN   ( IMM | STRING )
"    block     : 'block'   buffer  [ range ]
"    data      : 'data'    TOKEN   STRING
"
"    operand   : REG | MEM | IMM | reference
"    decorator : '@' TOKEN [ ':' TOKEN ]
"    reference : '&' TOKEN | '$' | '$$'
"    buffer    : STRING | SIZE reference
"    range     : ( IMM | reference ) '~' ( IMM | reference )
"

if exists("b:current_syntax")
    finish
endif

let s:cpo_save = &cpo
set cpo&vim

let zasm_asm_highlight=1

syn match   ZASMComment       '#.*$' contains=ZASMToDo,@Spell
syn keyword ZASMToDo          contained XXX TODO FIXME NOTE HACK
syn keyword ZASMKeyword       define block data
syn keyword ZASMStructure     byte word dword qword
syn keyword ZASMInclude       include

syn match   ZASMToken         '\w\+'
syn match   ZASMNumber        "\<\d\+\>"
syn match   ZASMNumber        "\<0x\x\+\>"
syn match   ZASMNumber        "\<0b\b\+\>"
syn match   ZASMNumber        "\<0o\o\+\>"
syn region  ZASMString        start=+\z(['"]\)+ end="\z1" skip="\\\z1" contains=ZASMEscape
syn match   ZASMEscape        contained +\\[abfnrtv0'"\\]+
syn match   ZASMEscape        contained +\\x\x\{2}+

syn match   ZASMLabel         '^\s*\w\+:'
syn match   ZASMDecorator     '@\w\+\%(\s*:\s*\w\+\)\='
syn match   ZASMRef           '\$\{1,2}'
syn match   ZASMRef           '&\w\+'

syn match   ZASMRange         '\~'

" ZASM highlight syntax definition
"

hi def link ZASMComment         Comment
hi def link ZASMToDo            Todo
hi def link ZASMKeyword         Statement
hi def link ZASMStructure       Structure
hi def link ZASMInclude         Include
hi def link ZASMToken           Function
hi def link ZASMNumber          Number
hi def link ZASMString          String
hi def link ZASMEscape          Special
hi def link ZASMLabel           Label
hi def link ZASMDecorator       Special
hi def link ZASMRef             Typedef
hi def link ZASMRange           Delimiter

let &cpo = s:cpo_save
unlet s:cpo_save

" vim: set spell!:
