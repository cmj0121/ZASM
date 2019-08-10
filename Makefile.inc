BASEDIR=$(shell git rev-parse --show-toplevel)
ZASM=$(BASEDIR)/zasm

ZASM_MAJOR=$(shell grep MAJOR $(BASEDIR)/include/zasm.h | awk '{print $$3}')
ZASM_MINOR=$(shell grep MINOR $(BASEDIR)/include/zasm.h | awk '{print $$3}')

CXXFLAGS=-Wall -std=c++11 -I$(BASEDIR)/include
LDFLAGS=

# vim: set ft=make:
