CC = cc
CFLAGS = -pedantic -std=c99 -Wall -Wextra

BIN =\
     cat\
     yes

all: ${BIN}

clean:
	rm -f ${BIN}

.PHONY: all clean
