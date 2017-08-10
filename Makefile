CC= g++
CFLAGS= --std=c++17

LDLIBS= -lcurl
LDFLAGS= -L/usr/lib/x86_64-linux-gnu

SRC= src/*.cpp
OUT= bin/cleverbot.out

build: ${SRC}
	${CC} ${CFLAGS} -o ${OUT} ${SRC} ${LDFLAGS} ${LDLIBS}

.PHONY: clean static

clean:
	rm ./bin/cleverbot.out
