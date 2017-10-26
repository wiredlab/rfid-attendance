


MDFILES=$(wildcard *.md)
PDFS=$(MDFILES:.md=.pdf)

default: $(PDFS)

%.pdf: %.md
	pandoc -o $@ $<
