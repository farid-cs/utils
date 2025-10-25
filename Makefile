CC = cc
CPPFLAGS = -D_XOPEN_SOURCE=700
CFLAGS = -pedantic -std=c99 -Wall -Wextra

BIN =\
     cat\
     false\
     true\
     yes

all: ${BIN}

${BIN}:
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -o $@

cat: cat.o
false: false.o
true: true.o
yes: yes.o

clean:
	rm -f *.o ${BIN}

.PHONY: all clean
