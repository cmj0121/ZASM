include Makefile.inc

SRC=zasm.cpp $(wildcard src/*.cpp)
OBJ=$(subst .cpp,.o,$(SRC))
BIN=zasm

SUBDIR=examples

VIMRC=$(wildcard *.vim)

.PHONY: all install test $(SUBDIR)

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

test: $(BIN) $(SUBDIR)
	$(MAKE) -C examples

install:
	install -m644 $(VIMRC) ~/.vim/syntax/

$(SUBDIR):
	$(DEBUG) ln -sf ../Makefile.inc $@/Makefile.inc

.PHONY: release
ZASM_MAJOR=$(shell grep MAJOR $(BASEDIR)/include/zasm.h | awk '{print $$3}')
ZASM_MINOR=$(shell grep MINOR $(BASEDIR)/include/zasm.h | awk '{print $$3}')

release: examples
	git tag v$(ZASM_MAJOR).$(ZASM_MINOR)
	git push v$(ZASM_MAJOR).$(ZASM_MINOR)

.PHONY: clean

clean: $(SUBDIR)
	rm -rf $(BIN) $(OBJ)
	$(MAKE) -C examples clean

