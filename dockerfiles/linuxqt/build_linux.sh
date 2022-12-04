#!/bin/bash

qmake6 -o Makefile.linux && \
	make -f Makefile.linux clean && \
	make -f Makefile.linux && \
	mkdir -p output/linux/docs && \
	mkdir -p output/linux/config && \
	cp -r docs/* output/linux/docs && \
	cp -r config/* output/linux/config
