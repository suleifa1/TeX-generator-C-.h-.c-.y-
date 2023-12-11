.PHONY: all clean test tex leaks pdf clean-docs clean-all

APP = ccdoc.exe

CC = gcc
OPTS = -Wall -pedantic -ansi 

LEXERSRC = lex.yy.c
PARSESRC = y.tab.c
PARSESRCH = y.tab.h
GEN_SRC = $(PARSESRC) $(PARSESRCH) $(LEXERSRC) 

PARSER = parser.y
DOC_DIR = docs
LATEX = $(DOC_DIR)/$(PARSER:%.y=%-doc.tex)
SRC = parserfuncs.c list.c func_param.c text.c $(filter %.c, $(GEN_SRC))
OBJ = $(SRC:%.c=%.o)


$(APP): $(OBJ)
	$(CC) $(OPTS) $^ -o $@    

%.o: %.c
	$(CC) $(OPTS) -c $^ -o $@    

$(LEXERSRC): scaner.l
	flex $<

$(PARSESRC): parser.y
	yacc --debug --verbose -H $<

tex: $(APP) $(PARSER)
	./$^ $(LATEX)

pdf: $(LATEX)
	pdflatex --output-directory $(DOC_DIR) $<
	pdflatex --output-directory $(DOC_DIR) $<

all: $(APP) tex pdf

test: $(APP) tiny.c
	./$^ 

leaks: $(APP) $(PARSER)
	valgrind --leak-check=yes ./$^ 

clean:
	rm -rf $(APP) $(GEN_SRC) y.output *.tex *.o *.pdf *.log

clean-docs:
	rm -rf $(DOC_DIR)/*

clean-all:
	$(MAKE) clean
	$(MAKE) clean-docs
