CXX := clang++
CXXFLAGS += \
	-std=c++11 \
	-fPIC \
	-fvisibility=hidden \
	-O3
LDFLAGS += \
	-framework Cocoa

SOURCES_COMMON += ${VST3_SDK_DIR}/public.sdk/source/main/macmain.cpp

SO_DIR := ${BUILD_PLUGIN_DIR}/Contents/MacOS
SO_FILE := ${SO_DIR}/${NAME}

EXTRA_RESOURCE_TARGETS += ${BUILD_PLUGIN_DIR}/Contents/PkgInfo

RESOURCE_TARGETS += ${EXTRA_RESOURCE_TARGETS}
ALL_TARGETS += ${SO_FILE} ${EXTRA_RESOURCE_TARGETS} ${BUILD_PLUGIN_DIR}/Contents/Info.plist
ALL_DIRS += ${SO_DIR} ${BUILD_PLUGIN_DIR}/Contents build/tmp

INSTALL_USER_PREFIX := ${HOME}/Library/Audio/Plug-Ins/VST3
INSTALL_PREFIX := /Library/Audio/Plug-Ins/VST3

CLEAN_RM += build/tmp

${SO_FILE}: build/tmp/${NAME}-x86_64 build/tmp/${NAME}-arm64 | ${SO_DIR}
	lipo -create -output $@ $^

build/tmp/${NAME}-x86_64: ${SOURCES} | build/tmp
	${CXX} $^ ${CXXFLAGS} ${LDFLAGS} -arch x86_64 -o $@

build/tmp/${NAME}-arm64: ${SOURCES} | build/tmp
	${CXX} $^ ${CXXFLAGS} ${LDFLAGS} -arch arm64 -o $@

${BUILD_PLUGIN_DIR}/Contents/Info.plist: ${TEMPLATE_DIR}/Contents/Info.plist | ${BUILD_PLUGIN_DIR}/Contents
	cat $^ | sed s:@NAME@:${NAME}:g > $@
