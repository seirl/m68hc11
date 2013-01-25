GCC := gcc
NAME := m68k
CFLAGS := -o ${NAME} -ansi -Wall -Wextra
SOURCES := main.c parser.c instr.c

all:
	${GCC} ${CFLAGS} ${SOURCES}

clean:
	rm ${NAME}
