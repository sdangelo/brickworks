ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
SOURCES = ${SOURCES_COMMON} ${ROOT_DIR}/../src/bw_example_fx_comb.c
NEEDS_MEMSET := yes

include ${ROOT_DIR}/../../common/web/web.mk
