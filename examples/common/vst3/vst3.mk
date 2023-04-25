default: all

COMMON_DIR := ${ROOT_DIR}/../../common/vst3
VST3_SDK_DIR := ${ROOT_DIR}/../../../../VST_SDK/vst3sdk
TEMPLATE_DIR := ${ROOT_DIR}/../../common/vst3/template.vst3
BUILD_PLUGIN_DIR := build/${NAME}.vst3

CXXFLAGS := \
	-DRELEASE=1 \
	-I${ROOT_DIR} \
	-I${ROOT_DIR}/../src \
	-I${COMMON_DIR} \
	-I${ROOT_DIR}/../../../include \
	-I${VST3_SDK_DIR} \
	-O3 \
	-Wall \
	-Wextra \
	-Wpedantic \
	-std=c++20
LDFLAGS := \
	-shared

SOURCES_COMMON := \
	${COMMON_DIR}/entry.cpp \
	${COMMON_DIR}/plugin.cpp \
	${COMMON_DIR}/controller.cpp \
	\
	${VST3_SDK_DIR}/base/source/fobject.cpp \
	${VST3_SDK_DIR}/base/source/baseiids.cpp \
	${VST3_SDK_DIR}/base/source/fstreamer.cpp \
	${VST3_SDK_DIR}/base/source/fstring.cpp \
	${VST3_SDK_DIR}/base/source/fbuffer.cpp \
	${VST3_SDK_DIR}/base/source/updatehandler.cpp \
	${VST3_SDK_DIR}/base/thread/source/flock.cpp \
	${VST3_SDK_DIR}/pluginterfaces/base/coreiids.cpp \
	${VST3_SDK_DIR}/pluginterfaces/base/funknown.cpp \
	${VST3_SDK_DIR}/pluginterfaces/base/ustring.cpp \
	${VST3_SDK_DIR}/pluginterfaces/base/conststringtable.cpp \
	${VST3_SDK_DIR}/public.sdk/source/main/pluginfactory.cpp \
	${VST3_SDK_DIR}/public.sdk/source/main/moduleinit.cpp \
	${VST3_SDK_DIR}/public.sdk/source/common/commoniids.cpp \
	${VST3_SDK_DIR}/public.sdk/source/common/pluginview.cpp \
	${VST3_SDK_DIR}/public.sdk/source/vst/vstcomponentbase.cpp \
	${VST3_SDK_DIR}/public.sdk/source/vst/vstcomponent.cpp \
	${VST3_SDK_DIR}/public.sdk/source/vst/vstaudioeffect.cpp \
	${VST3_SDK_DIR}/public.sdk/source/vst/vstinitiids.cpp \
	${VST3_SDK_DIR}/public.sdk/source/vst/vstbus.cpp \
	${VST3_SDK_DIR}/public.sdk/source/vst/vsteditcontroller.cpp \
	${VST3_SDK_DIR}/public.sdk/source/vst/vstparameters.cpp

RESOURCE_SOURCES := $(shell find ${TEMPLATE_DIR}/Contents/Resources/ -type f)
RESOURCE_FILES := $(shell printf '%s\n' ${RESOURCE_SOURCES} | sed s:^${TEMPLATE_DIR}/::g)
RESOURCE_TARGETS := $(addprefix ${BUILD_PLUGIN_DIR}/, ${RESOURCE_FILES})
RESOURCE_DIRS = $(sort $(dir ${RESOURCE_TARGETS}))

ALL_TARGETS := ${RESOURCE_TARGETS}
ALL_DIRS := ${RESOURCE_DIRS}

CLEAN_RM := \
	${BUILD_PLUGIN_DIR}

# OS detection scheme taken from https://stackoverflow.com/questions/714100/os-detecting-makefile
ifeq ($(OS), Windows_NT)
	include ${COMMON_DIR}/vst3-win32.mk
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		include ${COMMON_DIR}/vst3-linux.mk
	else ifeq ($(UNAME_S),Darwin)
		include ${COMMON_DIR}/vst3-macos.mk
	else
$(error OS not supported) # yes, GNU make complains about tabs...
	endif
endif

all: ${ALL_TARGETS}

.SECONDEXPANSION:

${RESOURCE_TARGETS}: ${BUILD_PLUGIN_DIR}/%: ${TEMPLATE_DIR}/% | $$(dir $${BUILD_PLUGIN_DIR}/$$*)
	cp ${TEMPLATE_DIR}/$* $@

${ALL_DIRS}:
	mkdir -p $@

install-user:
	mkdir -p ${INSTALL_USER_PREFIX}
	@echo mkdir -p -m 0755 `find ${BUILD_PLUGIN_DIR} -type d | sed s?^build/?${INSTALL_USER_PREFIX}/?g`
	@mkdir -p -m 0755 `find ${BUILD_PLUGIN_DIR} -type d | sed s?^build/?${INSTALL_USER_PREFIX}/?g`
	@for f in `find ${BUILD_PLUGIN_DIR} -type f | sed s?^build/??g`; do \
		d=`dirname $$f`; \
		echo install -m `[ -x build/$$f ] && echo 0755 || echo 0644` build/$$f ${INSTALL_USER_PREFIX}/$$d; \
		install -m `[ -x build/$$f ] && echo 0755 || echo 0644` build/$$f ${INSTALL_USER_PREFIX}/$$d; \
	done

install:
	mkdir -p ${INSTALL_PREFIX}
	@echo mkdir -p -m 0755 `find ${BUILD_PLUGIN_DIR} -type d | sed s?^build/?${INSTALL_PREFIX}/?g`
	@mkdir -p -m 0755 `find ${BUILD_PLUGIN_DIR} -type d | sed s?^build/?${INSTALL_PREFIX}/?g`
	@for f in `find ${BUILD_PLUGIN_DIR} -type f | sed s?^build/??g`; do \
		d=`dirname $$f`; \
		echo install -m `[ -x build/$$f ] && echo 0755 || echo 0644` build/$$f ${INSTALL_PREFIX}/$$d; \
		install -m `[ -x build/$$f ] && echo 0755 || echo 0644` build/$$f ${INSTALL_PREFIX}/$$d; \
	done


clean:
	rm -fr ${CLEAN_RM}

.PHONY: default all install-user install clean
