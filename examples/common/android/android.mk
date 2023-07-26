COMMON_DIR := ${ROOT_DIR}/../../common/android
BUILD_TOOLS_DIR := ${HOME}/Android/Sdk/build-tools/34.0.0
JAR_FILE := ${HOME}/Android/Sdk/platforms/android-34/android.jar

JAVAC := javac
KOTLINC := kotlinc
KEYTOOL := keytool
CXX := ${HOME}/Android/Sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi23-clang++
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

SOURCES_COMMON := \
	${COMMON_DIR}/jni.cpp

all: build/${NAME}.apk

build/${NAME}.apk: build/gen/${NAME}.aligned.apk build/apk/lib/armeabi-v7a/lib${NAME}.so build/gen/keystore.jks
	${APKSIGNER} sign --ks build/gen/keystore.jks --ks-key-alias androidkey --ks-pass pass:android --key-pass pass:android --out $@ build/gen/${NAME}.aligned.apk

build/gen/keystore.jks: | build/gen
	${KEYTOOL} -genkeypair -keystore $@ -alias androidkey -dname "CN=orastron.com, OU=ID, O=ORASTRON, L=Abc, S=Xyz, C=IT" -validity 10000 -keyalg RSA -keysize 2048 -storepass android -keypass android

build/gen/${NAME}.aligned.apk: build/gen/${NAME}.unsigned.apk
	${ZIPALIGN} -f -p 4 $^ $@

build/gen/${NAME}.unsigned.apk: build/apk/classes.dex build/gen/AndroidManifest.xml build/assets/index.html build/assets/config.js | build/gen
	#${AAPT} package -f -M build/gen/AndroidManifest.xml -I ${JAR_FILE} -F $@ build/apk
	#${AAPT} package -f -M build/gen/AndroidManifest.xml -S ${COMMON_DIR}/res -I ${JAR_FILE} -F $@ build/apk
	${AAPT} package -f -M build/gen/AndroidManifest.xml -A build/assets -I ${JAR_FILE} -F $@ build/apk
	
build/apk/classes.dex: build/apk/my_classes.jar
	cd build/apk && ${BUILD_TOOLS_DIR}/d8 ../../$^ ${JAR_FILE} && cd ../..

build/apk/my_classes.jar: build/obj/com/orastron/${NAME}/MainActivity.class | build/apk
	${D8} build/obj/com/orastron/${NAME}/MainActivity.class --output $@ --no-desugaring
	#${D8} $^ --output $@ --no-desugaring

build/apk/lib/armeabi-v7a/lib${NAME}.so: ${SOURCES} | build/apk/lib/armeabi-v7a
	${CXX} $^ ${CXXFLAGS} ${LDFLAGS} -o $@

build/obj/com/orastron/${NAME}/MainActivity.class: build/gen/com/orastron/${NAME}/MainActivity.java | build/obj
	${JAVAC} -classpath ${JAR_FILE} -d build/obj build/gen/com/orastron/${NAME}/MainActivity.java

build/gen/com/orastron/${NAME}/MainActivity.java: ${COMMON_DIR}/MainActivity.java | build/gen/com/orastron/${NAME}
	cat $^ | sed s:@NAME@:${NAME}:g > $@

build/gen/AndroidManifest.xml: ${COMMON_DIR}/AndroidManifest.xml | build/gen/com/orastron/${NAME}
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
