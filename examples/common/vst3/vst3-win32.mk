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

OBJ_DIR := build/obj
OBJ_COMMON := $(addprefix ${OBJ_DIR}/common/, $(SOURCES_COMMON:.cpp=.o))
OBJ_VST3_SDK := $(addprefix ${OBJ_DIR}/vst3/, $(SOURCES_VST3_SDK:.cpp=.o))

EXTRA_RESOURCE_TARGETS += ${BUILD_PLUGIN_DIR}/desktop.ini ${BUILD_PLUGIN_DIR}/Plugin.ico

RESOURCE_TARGETS += ${EXTRA_RESOURCE_TARGETS}
ALL_TARGETS += ${DLL_FILE} ${EXTRA_RESOURCE_TARGETS}
ALL_DIRS += ${DLL_DIR} ${BUILD_PLUGIN_DIR} ${OBJ_DIR} ${OBJ_DIR}/common

INSTALL_USER_PREFIX := $(shell echo '${LOCALAPPDATA}' | sed 's:\\:/:g')/Programs/Common/VST3

CLEAN_RM += ${OBJ_DIR}

${OBJ_DIR}/${NAME}.o: ${ROOT_DIR}/../src/${SOURCE} | ${OBJ_DIR}
	${CXX} ${ROOT_DIR}/../src/${SOURCE} ${CXXFLAGS} -Wall -Wextra -Wpedantic -c -o $@

${OBJ_DIR}/common/%.o: ${COMMON_DIR}/%.cpp | ${OBJ_DIR}/common
	${CXX} $^ ${CXXFLAGS} -c -o $@

${OBJ_DIR}/vst3/%.o: ${VST3_SDK_DIR}/%.cpp
	mkdir -p $(dir $@)
	${CXX} $^ ${CXXFLAGS} -c -o $@

${DLL_FILE}: ${OBJ_COMMON} ${OBJ_VST3_SDK} ${OBJ_DIR}/${NAME}.o ${SOURCES} | ${DLL_DIR}
	${CXX} $^ ${LDFLAGS} -o $@

