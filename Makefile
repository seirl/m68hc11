NAME := m68hc11

CC := gcc
CFLAGS := -ansi -Wall -Wextra -pedantic -std=c99

SRCDIR := src

ODIR := obj
_OBJ := main.o utils.o parser.o instr.o hashtbl.o list.o s19.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(ODIR)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	${CC} ${CFLAGS} -o ${NAME} $^

install:
	cp ${NAME} /usr/bin/

clean:
	rm ${NAME}
	rm -rf ${ODIR}
