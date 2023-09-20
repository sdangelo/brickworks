CXX := g++
CXXFLAGS += \
	-fPIC
LDFLAGS += \
	-static-libgcc \
	-static-libstdc++

SOURCES_VST3_SDK += \
	public.sdk/source/main/linuxmain.cpp

SO_DIR := ${BUILD_PLUGIN_DIR}/Contents/$(shell uname -m)-linux
SO_FILE := ${SO_DIR}/${NAME}.so

OBJ_DIR := build/obj
OBJ_COMMON := $(addprefix ${OBJ_DIR}/common/, $(SOURCES_COMMON:.cpp=.o))
OBJ_VST3_SDK := $(addprefix ${OBJ_DIR}/vst3/, $(SOURCES_VST3_SDK:.cpp=.o))

ALL_TARGETS += ${SO_FILE}
ALL_DIRS += ${SO_DIR} ${OBJ_DIR} ${OBJ_DIR}/common

INSTALL_USER_PREFIX := ${HOME}/.vst3
INSTALL_PREFIX := /usr/local/lib/vst3

CLEAN_RM += ${OBJ_DIR}

${OBJ_DIR}/${NAME}.o: ${ROOT_DIR}/../src/${SOURCE} | ${OBJ_DIR}
	${CXX} ${ROOT_DIR}/../src/${SOURCE} ${CXXFLAGS} -Wall -Wextra -Wpedantic -c -o $@

${OBJ_DIR}/common/%.o: ${COMMON_DIR}/%.cpp | ${OBJ_DIR}/common
	${CXX} $^ ${CXXFLAGS} -c -o $@

${OBJ_DIR}/vst3/%.o: ${VST3_SDK_DIR}/%.cpp
	mkdir -p $(dir $@)
	${CXX} $^ ${CXXFLAGS} -c -o $@

${SO_FILE}: ${OBJ_COMMON} ${OBJ_VST3_SDK} ${OBJ_DIR}/${NAME}.o | ${SO_DIR}
	${CXX} $^ ${LDFLAGS} -o $@
