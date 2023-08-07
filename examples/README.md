# Examples

Each subfolder contains an example application, except the `common` folder, which contains common code for all examples.

In order to build an example just `cd` to `*example*/*platofrm*` and use the following platform-specific instructions.

Building for any platform requires a recent enough version of [GNU Make](https://www.gnu.org/software/make/) installed.

## VST3

### Prerequisites

Windows (via [MSYS2/Mingw-w64](https://www.msys2.org/)), macOS, and Linux OSes are supported. Building tested with [GCC](https://gcc.gnu.org/), probably also works with [Clang](https://clang.llvm.org/).

You also need to download the [VST3 SDK](https://www.steinberg.net/developers/) and place it in the same folder as the Brickworks folder, or otherwise edit `common/vst3/vst3.mk` and change the `VST3_SDK_DIR` variable to point to the correct directory.

### Build

In order to build just type `make`. You'll find the resulting VST3 directory in `build/example.vst3`.

### Installation

If all went fine, you can install for the current user (i.e., into the user VST3 folder) by invoking `make install-user`.

On macOS and Linux you can also install for all users (i.e., into the system VST3 folder) by `make install`.

## Web

### Prerequisites

You need Clang with WebAssembly target support and [OpenSSL](https://www.openssl.org/) installed.

### Build

In order to build just type `make`. You'll find the resulting output directory in `build/web`.

### Running

The output files need to be served over HTTPS. A self-signed certificate is generated in the output directory (hence the OpenSSL requirement) to make it possible to run an HTTPS-enabled web server, e.g. `http-server -S`,  directly on/from the output folder.

## Daisy Seed

### Prerequisites

Building and firmware upload was only tested on Linux. You need [arm-none-eabi-gcc](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain) (for building) and [dfu-util](https://dfu-util.sourceforge.net/) (for firmware upload) installed.

You also need to clone [libDaisy](https://github.com/electro-smith/libDaisy), `cd` to it, and run `make`. You should either place it in the same folder as the Brickworks folder, or otherwise edit `common/daisy-seed/daisy-seed.mk` and change the `LIBDAISY_DIR` variable to point to the correct directory.

### Build

In order to build just type `make`. You'll find the resulting output files in `build`.

### Firmware upload

To upload the firmware:

1. branch the board to the uploading machine via USB;
2. put the board in DFU mode by pressing BOOT, then RESET, then letting go of RESET and then of BOOT;
3. type `make program-dfu`.

Effect examples report output parameter values and CPU usage statistics via USB serial. You can read the output by, e.g., `screen /dev/ttyACM0`.

## Android

### Prerequisites

Android examples are built without the help of Android Studio or Gradle. You'll however need to have a recent enough JDK (we need `javac` and `keytool`), as well as to download the latest stable:

- Android SDK (https://developer.android.com/studio/index.html) \*;
- Android NDK (https://developer.android.com/ndk/downloads) \*;
- `.jar`s and `.aar`s (and you'll also need to manually extract the inner `.jar` from each `.aar`, which are just ZIP files) for:
  - AndroidX Core (https://mvnrepository.com/artifact/androidx.core/core);
  - AndroidX Lifecycle Common (https://mvnrepository.com/artifact/androidx.lifecycle/lifecycle-common)
  - AndroidX VersionedParcelable (https://mvnrepository.com/artifact/androidx.versionedparcelable/versionedparcelable)
  - Kotlin Stdlib (https://mvnrepository.com/artifact/org.jetbrains.kotlin/kotlin-stdlib);
  - Koltin Coroutines Core (https://mvnrepository.com/artifact/org.jetbrains.kotlinx/kotlinx-coroutines-core);
  - Koltin Coroutines Core JVM (https://mvnrepository.com/artifact/org.jetbrains.kotlinx/kotlinx-coroutines-core-jvm);
- `miniaudio.h` library (http://miniaud.io/).

Then you'll probably also need to adjust paths in `common/android/android.mk`.

\* You can install both the needed parts of the Android SDK and the NDK by downloading the so-called "command line tools" (https://developer.android.com/studio#command-line-tools-only) and using the included `sdkmanager` program. In such case you need to install the following packages: "platforms;android-*latest*",  "build-tools;*latest*", "platform-tools", and "ndk;*latest*".

### Build

In order to build just type `make`. You'll find the resulting `.apk` file in `build`.

### Installation

If all went fine, you can branch your device and install using `make install`.

Otherwise, you can also install manually, but please remember to first uninstall the application from the device (`adb install -r` is not sufficient as the signature might have changed while building).

### Usage and known issues

Effect examples process audio input signals, therefore they will require permission to use the capture device.

Synth examples use input MIDI. While they are coded to support hotplugging, this doesn't seem to work as expected on the devices we tested. You'll need to press "STOP" and then "START" again after plugging a new device.

## iOS

### Prerequisites

iOS examples are not directly built by the supplied Makefiles. These rather generate the corresponding XCode projects and required files.

For this to work you need to have the latest [Xcode](https://developer.apple.com/xcode/) and [XcodeGen](https://github.com/yonaskolb/XcodeGen) installed, as well as a copy of the latest [`miniaudio.h`](http://miniaud.io/).

Finally, you might need to adjust header search path for miniaudio in `build/common/ios/project.yml`.

### Build

Typing `make` will generate the required Xcode project in `build/gen`.

You'll need to open it and select a development team (click on the project root in the left side pane, then choose the "Signing & Capabilities" tab, and finally pick a "Team").

At this point you can build and run as with any other iOS app.

### Usage

Effect examples process audio input signals, therefore they will require permission to use the capture device.

Synth examples use input MIDI and support hotplugging.

## Thanks

Thanks to:

- [Mads Kjeldgaard](https://madskjeldgaard.dk/) for publishing [instructions to build for the Daisy Seed and uploading the firmware](https://madskjeldgaard.dk/posts/daisy-setup/);
- [Hereket](https://hereket.github.io/) for providing instructions on [how to build an Android app without Android Studio or Gradle](https://hereket.github.io/posts/android_from_command_line/).
