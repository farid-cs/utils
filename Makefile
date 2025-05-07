CC = cc
CFLAGS = -pedantic -std=c99 -Wall -Wextra

BIN =\
     cat\
     false\
     true\
     yes

all: ${BIN}

clean:
	rm -f ${BIN}

.PHONY: all clean
