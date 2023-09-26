CC := gcc
CFLAGS := -I../include

all: build/bw_math
	./build/bw_math

build/bw_math: bw_math.c ../include/bw_math.h | build
	${CC} ${CFLAGS} bw_math.c -o $@

build:
	mkdir -p $@

clean:
	rm -fr build/

.PHONY: all
