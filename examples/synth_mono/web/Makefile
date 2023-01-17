ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
SOURCES = ${SOURCES_COMMON} ${ROOT_DIR}/../src/bw_example_synth_mono.c
SYNTH := yes
NEEDS_MEMSET := yes

include ${ROOT_DIR}/../../common/web/web.mk
