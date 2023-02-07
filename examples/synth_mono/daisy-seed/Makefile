ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

TARGET = bw_example_synth_mono

C_SOURCES += ${ROOT_DIR}/../src/bw_example_synth_mono.c

SYNTH := yes

include ${ROOT_DIR}/../../common/daisy-seed/daisy-seed.mk
