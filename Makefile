CC = cc
CPPFLAGS = -D_XOPEN_SOURCE=700
CFLAGS = -pedantic -std=c23 -Wall -Wextra -Wconversion

BIN =\
     cat\
     false\
     true\
     yes

all: ${BIN}

${BIN}:
	${CC} ${CFLAGS} ${CPPFLAGS} -o $@ $^

.c.o:
	${CC} ${CFLAGS} ${CPPFLAGS} -c -o $@ $^

cat: cat.o
false: false.o
true: true.o
yes: yes.o

clean:
	rm -f ${BIN:=.o} ${BIN}

.PHONY: all clean
