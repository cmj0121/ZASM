include Makefile.inc

ZASM=${BASEDIR}/zasm
SRC=$(wildcard *.zs)
CASE=$(subst .zs,.case,$(SRC))

.PHONY: all

all: $(CASE)

%.case: %.zs
	${ZASM} -f bin -o $@ $<
	@[ "`shasum -a 256 $@ | awk '{print $$1}'`" = "`sed -n '4p' $< | awk '{print $$3}'`" ]

.PHONY: clean

clean:
	rm -rf $(CASE)
