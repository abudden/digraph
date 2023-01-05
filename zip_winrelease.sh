#!/bin/bash

if [ $# -eq 0 ]
then
	source_dir=output/winrelease
	build=nb
elif [ "$1" == "native" ]
then
	source_dir=output/winrelease
	build=nb
elif [ "$1" == "winstatic" ]
then
	source_dir=output/winstatic
	build=static
elif [ "$1" == "winshared" ]
then
	source_dir=output/winshared
	build=dynamic
else
	echo "Unrecognised build type"
	exit 1
fi


if [ ! -e publish/windows ]
then
	mkdir -p publish/windows
fi

changeset=$(hg id -i)

if [[ ${changeset:0-1} == "+" ]]
then
	filename=digraph-windows-$(date +%Y%m%d)-${changeset}-${build}
else
	filename=digraph-windows-$(hg log -r . --template "{latesttag}-{latesttagdistance}-{node|short}")-$(date +%Y%m%d)-${build}
fi

if [ "$build" == "static" ]
then
	mv $source_dir digraph
	zip -r publish/windows/${filename}.zip digraph
	mv digraph $source_dir
else
	mv $source_dir digraph
	zip -r publish/windows/${filename}-all-dlls.zip digraph
	mv digraph $source_dir

	python filter_dlls.py $source_dir digraph
	zip -r publish/windows/${filename}-min-dlls.zip digraph
	rm -rf digraph
fi
