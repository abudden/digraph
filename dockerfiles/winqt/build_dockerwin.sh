#!/bin/bash
PATH=$PATH:/opt/mxe/usr/bin

for build in static shared
do

	MXETARGET=x86_64-w64-mingw32.${build}

	/opt/mxe/usr/${MXETARGET}/qt6/bin/qmake -o Makefile.win${build} "CONFIG+=dockerwin" "CONFIG+=win${build}" && \
		make -f Makefile.win${build} clean && \
		make -f Makefile.win${build} && \
		dockerfiles/winqt/mxedeployqt --mxetarget=${MXETARGET} output/win${build} && \
		mkdir -p output/win${build}/docs && \
		mkdir -p output/win${build}/config && \
		cp -r docs/* output/win${build}/docs && \
		cp -r config/* output/win${build}/config && \
		pandoc -s -o output/win${build}/README.html README.md && \
		bash zip_winrelease.sh win${build}
	if [ "$?" != "0" ]
	then
		exit 3
	fi
done
