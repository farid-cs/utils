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
	${CC} ${CPPFLAGS} ${CFLAGS} $@.c -o $@

clean:
	rm -f ${BIN}

.PHONY: all clean
