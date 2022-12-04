#!/bin/bash

if [ $# -eq 0 ]
then
	source_dir=output/winrelease
	build=nb
elif [ "$1" == "native" ]
then
	source_dir=output/winrelease
	build=nb
elif [ "$1" == "docker" ]
then
	source_dir=output/dockerwin
	build=dk
else
	echo "Unrecognised build type"
	exit 1
fi


mv $source_dir digraph
if [ ! -e publish/windows ]
then
	mkdir -p publish/windows
fi

changeset=$(hg id -i)

if [[ ${changeset:0-1} == "+" ]]
then
	filename=digraph-windows-$(date +%Y%m%d)-${changeset}-${build}.zip
else
	filename=digraph-windows-$(hg log -r . --template "{latesttag}-{latesttagdistance}-{node|short}")-$(date +%Y%m%d)-${build}.zip
fi
zip -r publish/windows/${filename} digraph
mv digraph $source_dir
