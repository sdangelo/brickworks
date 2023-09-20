default: all

COMMON_DIR := ${ROOT_DIR}/../../common/vst3
VST3_SDK_DIR := ${ROOT_DIR}/../../../../VST_SDK/vst3sdk
TEMPLATE_DIR := ${ROOT_DIR}/../../common/vst3/template.vst3
BUILD_PLUGIN_DIR := build/${NAME}.vst3

CXXFLAGS := \
	-I${ROOT_DIR} \
	-I${ROOT_DIR}/../src \
	-I${COMMON_DIR} \
	-I${ROOT_DIR}/../../../include \
	-I${VST3_SDK_DIR} \
	-O3 \
	-std=c++11
LDFLAGS := \
	-shared

SOURCES_COMMON := \
	entry.cpp \
	plugin.cpp \
	controller.cpp \

SOURCES_VST3_SDK := \
	base/source/fobject.cpp \
	base/source/baseiids.cpp \
	base/source/fstreamer.cpp \
	base/source/fstring.cpp \
	base/source/fbuffer.cpp \
	base/source/updatehandler.cpp \
	base/thread/source/flock.cpp \
	pluginterfaces/base/coreiids.cpp \
	pluginterfaces/base/funknown.cpp \
	pluginterfaces/base/ustring.cpp \
	pluginterfaces/base/conststringtable.cpp \
	public.sdk/source/main/pluginfactory.cpp \
	public.sdk/source/main/moduleinit.cpp \
	public.sdk/source/common/commoniids.cpp \
	public.sdk/source/common/pluginview.cpp \
	public.sdk/source/vst/vstcomponentbase.cpp \
	public.sdk/source/vst/vstcomponent.cpp \
	public.sdk/source/vst/vstaudioeffect.cpp \
	public.sdk/source/vst/vstinitiids.cpp \
	public.sdk/source/vst/vstbus.cpp \
	public.sdk/source/vst/vsteditcontroller.cpp \
	public.sdk/source/vst/vstparameters.cpp

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
