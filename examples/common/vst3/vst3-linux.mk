CXX := g++
CXXFLAGS += \
	-fPIC
LDFLAGS += \
	-static-libgcc \
	-static-libstdc++

SOURCES_COMMON += \
	${VST3_SDK_DIR}/public.sdk/source/main/linuxmain.cpp

SO_DIR := ${BUILD_PLUGIN_DIR}/Contents/$(shell uname -m)-linux
SO_FILE := ${SO_DIR}/${NAME}.so

ALL_TARGETS += ${SO_FILE}
ALL_DIRS += ${SO_DIR}

INSTALL_USER_PREFIX := ${HOME}/.vst3
INSTALL_PREFIX := /usr/local/lib/vst3

${SO_FILE}: ${SOURCES} | ${SO_DIR}
	${CXX} $^ ${CXXFLAGS} ${LDFLAGS} -o $@
