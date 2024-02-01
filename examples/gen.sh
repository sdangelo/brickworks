#!/bin/sh

TIBIA_DIR=~/repos/tibia

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

cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/cmd.json $TIBIA_DIR/templates/cmd cmd && cd ..
cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/cmd.json,../common/src/make.json,../common/src/cmd-make.json $TIBIA_DIR/templates/cmd-make cmd && cd ..
echo "include ../../common/cmd/Makefile" > fx_ap1/cmd/Makefile

cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json $TIBIA_DIR/templates/web web && cd ..
cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,../common/src/make.json,../common/src/web-make.json $TIBIA_DIR/templates/web-make web && cd ..
cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,../common/src/make.json,../common/src/web-make.json $TIBIA_DIR/templates/web-demo web && cd ..
echo "include ../../common/web/Makefile" > fx_ap1/web/Makefile

cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/daisy-seed.json $TIBIA_DIR/templates/daisy-seed daisy-seed && cd ..
cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/daisy-seed.json,../common/src/make.json,../common/src/daisy-seed-make.json $TIBIA_DIR/templates/daisy-seed-make daisy-seed && cd ..
echo "include ../../common/daisy-seed/Makefile" > fx_ap1/daisy-seed/Makefile

cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/lv2.json $TIBIA_DIR/templates/lv2 lv2 && cd ..
cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/lv2.json,../common/src/make.json,../common/src/lv2-make.json $TIBIA_DIR/templates/lv2-make lv2 && cd ..
echo "include ../../common/lv2/Makefile" > fx_ap1/lv2/Makefile

cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/vst3.json $TIBIA_DIR/templates/vst3 vst3 && cd ..
cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/vst3.json,../common/src/make.json,../common/src/vst3-make.json $TIBIA_DIR/templates/vst3-make vst3 && cd ..
echo "include ../../common/vst3/Makefile" > fx_ap1/vst3/Makefile

cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/android.json $TIBIA_DIR/templates/android android && cd ..
cd fx_ap1 && $TIBIA_DIR/tibia --data ../common/src/company.json,src/product.json,src/android.json,../common/src/make.json,../common/src/android-make.json $TIBIA_DIR/templates/android-make android && cd ..
echo "include ../../common/android/Makefile" > fx_ap1/android/Makefile
