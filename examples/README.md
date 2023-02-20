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

The output files need to be served over HTTPS. A self-signed certificate is generated in the output directory (hence the OpenSSL requirement) to make it possible to run an HTTPS-enabled web server, e.g. `https-server -S`,  directly on/from the output folder.

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

## Thanks

Thanks to [Mads Kjeldgaard](https://madskjeldgaard.dk/) for publishing [instructions to build for the Daisy Seed and uploading the firmware](https://madskjeldgaard.dk/posts/daisy-setup/).
