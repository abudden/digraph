#!/bin/sh
qmake-qt6 && make clean && make && bash get_dlls.sh output/winrelease
