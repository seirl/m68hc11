GCC := gcc
NAME := m68k
CFLAGS := -o ${NAME} -ansi -Wall -Wextra -pedantic -std=c99
SOURCES := main.c utils.c parser.c instr.c hashtbl.c

all:
	${GCC} ${CFLAGS} ${SOURCES}

clean:
	rm ${NAME}
