ARCH := x86_64
#ARCH := x86
#ARCH := arm64ec
#ARCH := arm
#ARCH := arm64

CXX := g++
CXXFLAGS += \
	-fPIC
LDFLAGS += \
	-static \
	-static-libgcc \
	-static-libstdc++ \
	-lole32

SOURCES_COMMON += ${VST3_SDK_DIR}/public.sdk/source/main/dllmain.cpp
	
DLL_DIR := ${BUILD_PLUGIN_DIR}/Contents/${ARCH}-win
DLL_FILE := ${DLL_DIR}/${NAME}.vst3

EXTRA_RESOURCE_TARGETS += ${BUILD_PLUGIN_DIR}/desktop.ini ${BUILD_PLUGIN_DIR}/Plugin.ico

RESOURCE_TARGETS += ${EXTRA_RESOURCE_TARGETS}
ALL_TARGETS += ${DLL_FILE} ${EXTRA_RESOURCE_TARGETS}
ALL_DIRS += ${DLL_DIR} ${BUILD_PLUGIN_DIR}

${DLL_FILE}: ${SOURCES} | ${DLL_DIR}
	${CXX} $^ ${CXXFLAGS} ${LDFLAGS} -o $@

#USER_PREFIX=$(shell echo '${LOCALAPPDATA}' | sed 's:\\:/:g')

#install-user: all
#	mkdir -p ${USER_PREFIX}/Programs/Common/VST3
#	@echo mkdir -p -m 0755 `find build/${NAME}.vst3 -type d | sed s?^build/?${USER_PREFIX}/Programs/Common/VST3/?g`
#	@mkdir -p -m 0755 `find build/${NAME}.vst3 -type d | sed s?^build/?${USER_PREFIX}/Programs/Common/VST3/?g`
#	@for f in `find build/${NAME}.vst3 -type f | sed s?^build/??g`; do \
#		d=`dirname $$f`; \
#		echo install -m `[ -x build/$$f ] && echo 0755 || echo 0644` build/$$f ${USER_PREFIX}/Programs/Common/VST3/$$d; \
#		install -m `[ -x build/$$f ] && echo 0755 || echo 0644` build/$$f ${USER_PREFIX}/Programs/Common/VST3/$$d; \
#	done
