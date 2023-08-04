COMMON_DIR := ${ROOT_DIR}/../../common/ios

COMMON_SOURCES := index.html app.swift native.mm app-Bridging-Header.h
COMMON_SOURCES_IN := $(addprefix ${COMMON_DIR}/, ${COMMON_SOURCES})
COMMON_SOURCES_OUT := $(addprefix build/gen/src/, ${COMMON_SOURCES})

SOURCES := ${SOURCE} config.js
SOURCES_IN := $(addprefix ${ROOT_DIR}/../src/, ${SOURCES})
SOURCES_OUT := $(addprefix build/gen/src/, ${SOURCES})

BUNDLE_NAME := $(shell echo ${NAME} | sed 's:_:-:g')

all: ${COMMON_SOURCES_OUT} ${SOURCES_OUT} build/gen/${NAME}.xcodeproj

${COMMON_SOURCES_OUT} ${SOURCES_OUT}: ${COMMON_SOURCES_IN} ${SOURCES_IN} | build/gen/src
	cp $^ build/gen/src

build/gen/${NAME}.xcodeproj: build/gen/project.yml
	xcodegen generate --spec $^

build/gen/project.yml: ${COMMON_DIR}/project.yml | build/gen
	cat $^ | sed s:@NAME@:${NAME}:g | sed s:@BUNDLE_NAME@:${BUNDLE_NAME}:g > $@

build/gen build/gen/src:
	mkdir -p $@

clean:
	rm -fr build

install: 

.PHONY: all clean install
