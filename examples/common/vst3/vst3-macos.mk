CXX := clang++
CXXFLAGS += \
	-std=c++11 \
	-fPIC \
	-fvisibility=hidden \
	-O3
LDFLAGS += \
	-framework Cocoa

SOURCES_VST3_SDK += \
	public.sdk/source/main/macmain.cpp

SO_DIR := ${BUILD_PLUGIN_DIR}/Contents/MacOS
SO_FILE := ${SO_DIR}/${NAME}

OBJ_DIR := build/obj
OBJ_DIR_X86_64 := ${OBJ_DIR}/x86_64
OBJ_DIR_ARM64 := ${OBJ_DIR}/arm64
OBJ_COMMON_X86_64 := $(addprefix ${OBJ_DIR_X86_64}/common/, $(SOURCES_COMMON:.cpp=.o))
OBJ_VST3_SDK_X86_64 := $(addprefix ${OBJ_DIRX86_64}/vst3/, $(SOURCES_VST3_SDK:.cpp=.o))
OBJ_COMMON_ARM64 := $(addprefix ${OBJ_DIR_ARM64}/common/, $(SOURCES_COMMON:.cpp=.o))
OBJ_VST3_SDK_ARM64 := $(addprefix ${OBJ_DIR_ARM64}/vst3/, $(SOURCES_VST3_SDK:.cpp=.o))

EXTRA_RESOURCE_TARGETS += ${BUILD_PLUGIN_DIR}/Contents/PkgInfo

RESOURCE_TARGETS += ${EXTRA_RESOURCE_TARGETS}
ALL_TARGETS += ${SO_FILE} ${EXTRA_RESOURCE_TARGETS} ${BUILD_PLUGIN_DIR}/Contents/Info.plist
ALL_DIRS += ${SO_DIR} ${BUILD_PLUGIN_DIR}/Contents ${OBJ_DIR} ${OBJ_DIR_X86_64} ${OBJ_DIR_ARM64} ${OBJ_DIR_X86_64}/common ${OBJ_DIR_ARM64}/common build/tmp

INSTALL_USER_PREFIX := ${HOME}/Library/Audio/Plug-Ins/VST3
INSTALL_PREFIX := /Library/Audio/Plug-Ins/VST3

CLEAN_RM += ${OBJ_DIR} build/tmp

${OBJ_DIR_X86_64}/${NAME}.o: ${ROOT_DIR}/../src/${SOURCE} | ${OBJ_DIR_X86_64}
	${CXX} ${ROOT_DIR}/../src/${SOURCE} ${CXXFLAGS} -arch x86_64 -Wall -Wextra -Wpedantic -c -o $@

${OBJ_DIR_X86_64}/common/%.o: ${COMMON_DIR}/%.cpp | ${OBJ_DIR_X86_64}/common
	${CXX} $^ ${CXXFLAGS} -arch x86_64 -c -o $@

${OBJ_DIR_X86_64}/vst3/%.o: ${VST3_SDK_DIR}/%.cpp
	mkdir -p $(dir $@)
	${CXX} $^ ${CXXFLAGS} -arch x86_64 -c -o $@

${OBJ_DIR_ARM64}/${NAME}.o: ${ROOT_DIR}/../src/${SOURCE} | ${OBJ_DIR_ARM64}
	${CXX} ${ROOT_DIR}/../src/${SOURCE} ${CXXFLAGS} -arch arm64 -Wall -Wextra -Wpedantic -c -o $@

${OBJ_DIR_ARM64}/common/%.o: ${COMMON_DIR}/%.cpp | ${OBJ_DIR_ARM64}/common
	${CXX} $^ ${CXXFLAGS} -arch arm64 -c -o $@

${OBJ_DIR_ARM64}/vst3/%.o: ${VST3_SDK_DIR}/%.cpp
	mkdir -p $(dir $@)
	${CXX} $^ ${CXXFLAGS} -arch arm64 -c -o $@

build/tmp/${NAME}-x86_64: ${OBJ_COMMON_ARM64} ${OBJ_VST3_SDK_X86_64} ${OBJ_DIR_X86_64}/${NAME}.o | build/tmp
	${CXX} $^ ${LDFLAGS} -arch x86_64 -o $@

build/tmp/${NAME}-arm64: ${OBJ_COMMON_ARM64} ${OBJ_VST3_SDK_ARM64} ${OBJ_DIR_ARM64}/${NAME}.o | build/tmp
	${CXX} $^ ${LDFLAGS} -arch arm64 -o $@

${SO_FILE}: build/tmp/${NAME}-x86_64 build/tmp/${NAME}-arm64 | ${SO_DIR}
	lipo -create -output $@ $^

${BUILD_PLUGIN_DIR}/Contents/Info.plist: ${TEMPLATE_DIR}/Contents/Info.plist | ${BUILD_PLUGIN_DIR}/Contents
	cat $^ | sed s:@NAME@:${NAME}:g > $@
