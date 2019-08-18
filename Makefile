include Makefile.inc

SRC=zasm.cpp $(wildcard src/*.cpp)
OBJ=$(subst .cpp,.o,$(SRC))
BIN=zasm

SUBDIR=examples

VIMRC=$(wildcard *.vim)

.PHONY: all install test

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

test: $(BIN)
	$(DEBUG) ln -sf ../Makefile.inc examples/Makefile.inc
	$(MAKE) -C examples

install:
	install -m644 $(VIMRC) ~/.vim/syntax/

.PHONY: release
ZASM_MAJOR=$(shell grep MAJOR $(BASEDIR)/include/zasm.h | awk '{print $$3}')
ZASM_MINOR=$(shell grep MINOR $(BASEDIR)/include/zasm.h | awk '{print $$3}')

release: examples
	git tag v$(ZASM_MAJOR).$(ZASM_MINOR)
	git push v$(ZASM_MAJOR).$(ZASM_MINOR)

.PHONY: clean

clean:
	rm -rf $(BIN) $(OBJ)
	$(MAKE) -C examples clean

