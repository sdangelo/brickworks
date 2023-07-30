ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

NAME := bw_example_synthpp_mono
SOURCES = ${SOURCES_COMMON} ${ROOT_DIR}/../src/bw_example_synthpp_mono.cpp
SYNTH := yes

include ${ROOT_DIR}/../../common/android/android.mk
