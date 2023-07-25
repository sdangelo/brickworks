LIBDAISY_DIR := ${ROOT_DIR}/../../../../libDaisy

ifdef SYNTH
CPP_SOURCES = ${ROOT_DIR}/../../common/daisy-seed/synth.cpp ${CPP_SOURCES_EXTRA}
else
CPP_SOURCES = ${ROOT_DIR}/../../common/daisy-seed/fx.cpp ${CPP_SOURCES_EXTRA}
endif

LDFLAGS += -u _printf_float

SYSTEM_FILES_DIR = ${LIBDAISY_DIR}/core
include ${SYSTEM_FILES_DIR}/Makefile

CPPFLAGS += \
	-DNDEBUG \
	-I${ROOT_DIR} \
	-I${ROOT_DIR}/../src \
	-I${ROOT_DIR}/../../../include

CFLAGS += \
	-DNDEBUG \
	-I${ROOT_DIR} \
	-I${ROOT_DIR}/../src \
	-I${ROOT_DIR}/../../../include \
	-I${ROOT_DIR}/../../common/daisy-seed
