#!/bin/sh
qmake-qt6 "CONFIG+=console" && make
if [ "$?" == 0 ]
then
	if [ ! -e output/winconsole/libiconv-2.dll ]
	then
		bash get_dlls.sh output/winconsole
	fi

	./output/winconsole/digraph.exe "$@"
fi
