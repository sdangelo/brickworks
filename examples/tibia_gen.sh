#!/bin/sh

TIBIA_DIR=../../../tibia

if [ -z "$1" -o "$1" = "common" ]; then
	echo Generating common files

	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/cmd cmd && cd ..
	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/cmd-make cmd && cd ..

	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/web web && cd ..
	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/web-make web && cd ..
	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/web-demo web && cd ..

	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/daisy-seed daisy-seed && cd ..
	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/daisy-seed-make daisy-seed && cd ..

	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/lv2 lv2 && cd ..
	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/lv2-make lv2 && cd ..

	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/vst3 vst3 && cd ..
	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/vst3-make vst3 && cd ..

	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/android android && cd ..
	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/android-make android && cd ..

	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/ios ios && cd ..
	cd common && $TIBIA_DIR/tibia --common $TIBIA_DIR/templates/ios-make ios && cd ..
fi

if [ "$1" = "common" ]; then
	exit 0
fi

if [ -z "$1" ]; then
	dirs="fx* synth*"
else
	dirs="$1"
fi

for d in $dirs; do
	echo Generating data files for $d

	case $d in
	fxpp*)
		make_json=make-cxx-fx.json
		;;
	synthpp*)
		make_json=make-cxx-synth.json
		;;
	*)
		make_json=make.json
		;;
	esac

	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/cmd.json $TIBIA_DIR/templates/cmd cmd && cd ..
	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/cmd.json,../common/src/$make_json,../common/src/cmd-make.json $TIBIA_DIR/templates/cmd-make cmd && cd ..
	echo "include ../../common/cmd/Makefile" > $d/cmd/Makefile

	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json $TIBIA_DIR/templates/web web && cd ..
	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,../common/src/$make_json,../common/src/web-make.json $TIBIA_DIR/templates/web-make web && cd ..
	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,../common/src/$make_json,../common/src/web-make.json $TIBIA_DIR/templates/web-demo web && cd ..
	echo "include ../../common/web/Makefile" > $d/web/Makefile

	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/daisy-seed.json $TIBIA_DIR/templates/daisy-seed daisy-seed && cd ..
	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/daisy-seed.json,../common/src/$make_json,../common/src/daisy-seed-make.json $TIBIA_DIR/templates/daisy-seed-make daisy-seed && cd ..
	echo "include ../../common/daisy-seed/Makefile" > $d/daisy-seed/Makefile

	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/lv2.json $TIBIA_DIR/templates/lv2 lv2 && cd ..
	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/lv2.json,../common/src/$make_json,../common/src/lv2-make.json $TIBIA_DIR/templates/lv2-make lv2 && cd ..
	echo "include ../../common/lv2/Makefile" > $d/lv2/Makefile

	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/vst3.json $TIBIA_DIR/templates/vst3 vst3 && cd ..
	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/vst3.json,../common/src/$make_json,../common/src/vst3-make.json $TIBIA_DIR/templates/vst3-make vst3 && cd ..
	echo "include ../../common/vst3/Makefile" > $d/vst3/Makefile

	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/android.json $TIBIA_DIR/templates/android android && cd ..
	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/android.json,../common/src/$make_json,../common/src/android-make.json $TIBIA_DIR/templates/android-make android && cd ..
	echo "include ../../common/android/Makefile" > $d/android/Makefile

	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/ios.json $TIBIA_DIR/templates/ios ios && cd ..
	cd $d && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/ios.json,../common/src/ios-$make_json $TIBIA_DIR/templates/ios-make ios && cd ..
	echo "include ../../common/ios/Makefile" > $d/ios/Makefile
done
