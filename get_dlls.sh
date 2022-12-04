#!/bin/bash

windeployqt-qt6.exe "$1"

echo ""
echo "Getting DLLs"

ldd `find "$1" -iname '*.dll' -o -iname '*.exe'` \
	| grep -iv system32 \
	| grep -vi windows \
	| grep -v :$ \
	| cut -f2 -d\> \
	| cut -f1 -d\( \
	| tr '\\' '/' \
	| while read a; do ! [ -e "$1/$(basename "$a")" ] && cp -v "$a" $1/; done

mkdir -p $1/docs
mkdir -p $1/config

cp -r docs/* $1/docs/
cp -r config/* $1/config/
