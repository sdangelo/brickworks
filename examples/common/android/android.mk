COMMON_DIR := ${ROOT_DIR}/../../common/android
ANDROID_SDK_DIR := ${HOME}/Android/Sdk
ANDROID_NDK_DIR := ${ANDROID_SDK_DIR}/ndk/25.2.9519653
ANDROIDX_DIR := ${HOME}/Android/androidx
KOTLIN_DIR := ${HOME}/Android/kotlin
MINIAUDIO_DIR := ${ROOT_DIR}/../../../../miniaudio

BUILD_TOOLS_DIR := ${ANDROID_SDK_DIR}/build-tools/34.0.0

JAR_FILE := ${ANDROID_SDK_DIR}/platforms/android-34/android.jar
ANDROIDX_CORE_FILE := ${ANDROIDX_DIR}/core-1.10.1.jar
ANDROIDX_LIFECYCLE_COMMON_FILE := ${ANDROIDX_DIR}/lifecycle-common-2.6.1.jar
ANDROIDX_VERSIONEDPARCELABLE_FILE := ${ANDROIDX_DIR}/versionedparcelable-1.1.1.jar
KOTLIN_STDLIB_FILE := ${KOTLIN_DIR}/kotlin-stdlib-1.9.0.jar
KOTLINX_COROUTINES_CORE_FILE := ${KOTLIN_DIR}/kotlinx-coroutines-core-1.7.3.jar
KOTLINX_COROUTINES_CORE_JVM_FILE := ${KOTLIN_DIR}/kotlinx-coroutines-core-jvm-1.7.3.jar

JAVAC := javac
KEYTOOL := keytool
ifdef SYNTH
CXX := ${ANDROID_NDK_DIR}/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi29-clang++
else
CXX := ${ANDROID_NDK_DIR}/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi26-clang++
endif
ADB := ${HOME}/Android/Sdk/platform-tools/adb
APKSIGNER := ${BUILD_TOOLS_DIR}/apksigner
ZIPALIGN := ${BUILD_TOOLS_DIR}/zipalign
AAPT := ${BUILD_TOOLS_DIR}/aapt
D8 := ${BUILD_TOOLS_DIR}/d8

CXXFLAGS := \
	-fPIC \
	-DNDEBUG \
	-I${ROOT_DIR}/../src \
	-I${COMMON_DIR} \
	-I${ROOT_DIR}/../../../include \
	-I${MINIAUDIO_DIR} \
	-O3 \
	-Wall \
	-Wextra \
	-Wpedantic \
	-std=c++20
LDFLAGS := \
	-shared \
	-static-libstdc++ \
	-ljnigraphics \
	-llog \
	-landroid

ifdef SYNTH
LDFLAGS += -lamidi
endif

SOURCES_COMMON := \
	build/gen/jni.cpp

JARS := \
	${JAR_FILE} \
	${ANDROIDX_CORE_FILE} \
	${ANDROIDX_LIFECYCLE_COMMON_FILE} \
	${ANDROIDX_VERSIONEDPARCELABLE_FILE} \
	${KOTLIN_STDLIB_FILE} \
	${KOTLINX_COROUTINES_CORE_FILE} \
	${KOTLINX_COROUTINES_CORE_JVM_FILE}

JNI_NAME := $(shell echo ${NAME} | sed 's:_:_1:g')

ifdef SYNTH
ANDROID_MANIFEST_SOURCE := ${COMMON_DIR}/AndroidManifest-synth.xml
MAIN_ACTIVITY_SOURCE := ${COMMON_DIR}/MainActivity-synth.java
else
ANDROID_MANIFEST_SOURCE := ${COMMON_DIR}/AndroidManifest-fx.xml
MAIN_ACTIVITY_SOURCE := ${COMMON_DIR}/MainActivity-fx.java
endif

all: build/${NAME}.apk

build/${NAME}.apk: build/gen/${NAME}.aligned.apk build/apk/lib/armeabi-v7a/lib${NAME}.so build/gen/keystore.jks
	${APKSIGNER} sign --ks build/gen/keystore.jks --ks-key-alias androidkey --ks-pass pass:android --key-pass pass:android --out $@ build/gen/${NAME}.aligned.apk

build/gen/keystore.jks: | build/gen
	${KEYTOOL} -genkeypair -keystore $@ -alias androidkey -dname "CN=orastron.com, OU=ID, O=ORASTRON, L=Abc, S=Xyz, C=IT" -validity 10000 -keyalg RSA -keysize 2048 -storepass android -keypass android

build/gen/${NAME}.aligned.apk: build/gen/${NAME}.unsigned.apk
	${ZIPALIGN} -f -p 4 $^ $@

build/gen/${NAME}.unsigned.apk: build/apk/classes.dex build/gen/AndroidManifest.xml build/assets/index.html build/assets/config.js build/apk/lib/armeabi-v7a/lib${NAME}.so| build/gen
	${AAPT} package -f -M build/gen/AndroidManifest.xml -A build/assets -I ${JAR_FILE} -I ${ANDROIDX_CORE_FILE} -I ${ANDROIDX_LIFECYCLE_COMMON_FILE} -I ${ANDROIDX_VERSIONEDPARCELABLE_FILE} -I ${KOTLIN_STDLIB_FILE} -I ${KOTLINX_COROUTINES_CORE_FILE} -I ${KOTLINX_COROUTINES_CORE_JVM_FILE} -F $@ build/apk
	
ifdef SYNTH
build/apk/classes.dex: build/apk/my_classes.jar
	cd build/apk && ${D8} --min-api 29 ../../$^ ${JARS} && cd ../..

build/apk/my_classes.jar: build/obj/com/orastron/${NAME}/MainActivity.class build/obj/com/orastron/${NAME}/MainActivity$$WebAppInterface.class build/obj/com/orastron/${NAME}/MainActivity$$WebAppInterface$$MidiDeviceCallback.class build/obj/com/orastron/${NAME}/MainActivity$$WebAppInterface$$1.class | build/apk
	${D8} build/obj/com/orastron/${NAME}/MainActivity.class 'build/obj/com/orastron/${NAME}/MainActivity$$WebAppInterface.class' 'build/obj/com/orastron/${NAME}/MainActivity$$WebAppInterface$$MidiDeviceCallback.class' 'build/obj/com/orastron/${NAME}/MainActivity$$WebAppInterface$$1.class' --min-api 29 --output $@ --no-desugaring
else
build/apk/classes.dex: build/apk/my_classes.jar
	cd build/apk && ${D8} --min-api 26 ../../$^ ${JARS} && cd ../..

build/apk/my_classes.jar: build/obj/com/orastron/${NAME}/MainActivity.class build/obj/com/orastron/${NAME}/MainActivity$$WebAppInterface.class | build/apk
	${D8} build/obj/com/orastron/${NAME}/MainActivity.class 'build/obj/com/orastron/${NAME}/MainActivity$$WebAppInterface.class' --min-api 26 --output $@ --no-desugaring
endif

build/apk/lib/armeabi-v7a/lib${NAME}.so: ${SOURCES} | build/apk/lib/armeabi-v7a
	${CXX} $^ ${CXXFLAGS} ${LDFLAGS} -o $@
	
build/gen/jni.cpp: ${COMMON_DIR}/jni.cpp | build/gen
	cat $^ | sed s:@JNI_NAME@:${JNI_NAME}:g > $@

build/obj/com/orastron/${NAME}/MainActivity$$WebAppInterface.class: build/obj/com/orastron/${NAME}/MainActivity.class

build/obj/com/orastron/${NAME}/MainActivity.class: build/gen/com/orastron/${NAME}/MainActivity.java | build/obj
	${JAVAC} -classpath "${JAR_FILE}:${ANDROIDX_CORE_FILE}:${ANDROIDX_LIFECYCLE_COMMON_FILE}:${ANDROIDX_VERSIONEDPARCELABLE_FILE}:${KOTLIN_STDLIB_FILE}:${KOTLINX_COROUTINES_CORE_FILE}:${KOTLINX_COROUTINES_CORE_JVM_FILE}" -d build/obj build/gen/com/orastron/${NAME}/MainActivity.java

build/gen/com/orastron/${NAME}/MainActivity.java: ${MAIN_ACTIVITY_SOURCE} | build/gen/com/orastron/${NAME}
	cat $^ | sed s:@NAME@:${NAME}:g > $@

build/gen/AndroidManifest.xml: ${ANDROID_MANIFEST_SOURCE} | build/gen/com/orastron/${NAME}
	cat $^ | sed s:@NAME@:${NAME}:g > $@

build/assets/index.html: ${COMMON_DIR}/index.html | build/assets
	cp $^ $@

build/assets/config.js: ${ROOT_DIR}/../src/config.js | build/assets
	cp $^ $@

build/apk build/apk/lib/armeabi-v7a build/obj build/gen/com/orastron/${NAME} build/gen build/assets:
	mkdir -p $@

clean:
	rm -fr build

install: build/${NAME}.apk
	[ -n "`${ADB} shell pm list packages | grep ^package:com.orastron.${NAME}$$`" ] && ${ADB} uninstall com.orastron.${NAME}; exit 0
	${ADB} install $^

.PHONY: all clean install
