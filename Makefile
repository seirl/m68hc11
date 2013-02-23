NAME := m68hc11
CC := gcc
CFLAGS := -o ${NAME} -ansi -Wall -Wextra -pedantic -std=c99
SOURCES := main.c utils.c parser.c instr.c hashtbl.c list.c s19.c

all:
	${CC} ${CFLAGS} ${SOURCES}

clean:
	rm ${NAME}
