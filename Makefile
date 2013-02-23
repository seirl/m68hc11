NAME := m68hc11

CC := gcc
CFLAGS := -ansi -Wall -Wextra -pedantic -std=c99

ODIR := obj
_OBJ := main.o utils.o parser.o instr.o hashtbl.o list.o s19.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

DEPS := core.h hashtbl.h instr.h list.h parser.h s19.h utils.h


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	${CC} ${CFLAGS} -o ${NAME} $^ -I.

clean:
	rm ${NAME}
	rm -rf ${ODIR}
