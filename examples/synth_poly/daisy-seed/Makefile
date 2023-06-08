ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

TARGET = bw_example_synth_poly

C_SOURCES += ${ROOT_DIR}/../src/bw_example_synth_poly.c

SYNTH := yes

include ${ROOT_DIR}/../../common/daisy-seed/daisy-seed.mk
