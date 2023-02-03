LIBDAISY_DIR := ${ROOT_DIR}/../../../../libDaisy

ifdef SYNTH
CPP_SOURCES = ${ROOT_DIR}/../../common/daisy-seed/synth.cpp
else
CPP_SOURCES = ${ROOT_DIR}/../../common/daisy-seed/fx.cpp
endif

LDFLAGS += -u _printf_float

SYSTEM_FILES_DIR = ${LIBDAISY_DIR}/core
include ${SYSTEM_FILES_DIR}/Makefile

CPPFLAGS += \
	-I${ROOT_DIR}/../src \
	-I${ROOT_DIR}/../../../include
