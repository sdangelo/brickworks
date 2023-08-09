CC := gcc
CFLAGS := -I../include

all: build/bw_math
	./build/bw_math

build/bw_math: bw_math.c ../include/bw_math.h | build
	${CC} ${CFLAGS} $^ -o $@

build:
	mkdir -p $@

clean:
	rm -fr build/

.PHONY: all
