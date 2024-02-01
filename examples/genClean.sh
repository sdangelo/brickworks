#!/bin/sh

echo Removing common files

rm -fr common/cmd common/web common/daisy-seed common/lv2 common/vst3 common/android

dirs="fx_ap1 fx_ap2"

for d in $dirs; do
	echo Removing data files for $d

	rm -fr $d/cmd $d/web $d/daisy-seed $d/lv2 $d/vst3 $d/android
done
