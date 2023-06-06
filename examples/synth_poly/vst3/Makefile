ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

NAME := bw_example_synth_mono
SOURCES = ${SOURCES_COMMON} ${ROOT_DIR}/../src/bw_example_synth_mono.c

include ${ROOT_DIR}/../../common/vst3/vst3.mk
