ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

NAME := bw_example_fxpp_balance
SOURCES = ${SOURCES_COMMON} ${ROOT_DIR}/../src/bw_example_fxpp_balance.cpp

include ${ROOT_DIR}/../../common/vst3/vst3.mk

CXXFLAGS += -DRELEASE=1 -DNDEBUG -DBW_NO_DEBUG
#CXXFLAGS += -DDEVELOPMENT=1 -DBW_DEBUG_DEEP
