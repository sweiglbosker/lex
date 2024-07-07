TARGET=lex
SRC=fsm.c dfa.c nfa.c main.c regex.c
OBJ=$(SRC:.c=.o)
HEADERS = $(wildcard *.h)
CFLAGS := -Wall -Wshadow -g -march=native -std=c11 -pipe

${TARGET}: ${OBJ}
	${CC} ${CFLAGS} -o $@ $^

${OBJ}: ${SRC} ${HEADERS}

clean:
	rm -rf ${TARGET} ${OBJ}

.PHONY=clean
