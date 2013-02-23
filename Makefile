NAME := m68hc11

CC := gcc
CFLAGS := -ansi -Wall -Wextra -pedantic -std=c99

SRCDIR := src

ODIR := obj
_OBJ := main.o utils.o parser.o instr.o hashtbl.o list.o s19.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

DEPDIR := include
_DEPS := core.h hashtbl.h instr.h list.h parser.h s19.h utils.h
DEPS = $(patsubst %,$(DEPDIR)/%,$(_DEPS))


$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) -g -c -o $@ $< $(CFLAGS) -I$(DEPDIR)

all: $(OBJ)
	${CC} ${CFLAGS} -o ${NAME} $^ -I$(DEPDIR)

install:
	cp ${NAME} /usr/bin/

clean:
	rm ${NAME}
	rm -rf ${ODIR}
