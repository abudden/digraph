#!/usr/bin/python

import glob
import os
import sys
import shutil

# windeployqt-qt6 and mxedeployqt etc get everything that could possibly be needed.
# In practice a lot of these are unnecessary.  Largely through trial-and-error, this list
# was produced to give a minimum viable subset to reduce the size of the output.

# First argument is source folder; second is output folder name
source_path = sys.argv[1]
destination_path = sys.argv[2]

if 'winstatic' in source_path:
    # Static version is smaller (and better!)
    needed_output_files = [
            'config/*', 'docs/*',
            'digraph.exe', 'README.html',
            ]
else:
    needed_output_files = [
            'config/*', 'docs/*',
            'plugins/platforms/qwindows.dll',
            'icudt66.dll', 'icuin66.dll', 'icuuc66.dll',
            'libbz2.dll', 'libfreetype*.dll',
            'libgcc_s_seh*.dll', 'libglib-*.dll',
            'libharfbuzz-*.dll', 'libiconv-*.dll', 'libintl-*.dll',
            'libpcre-*.dll', 'libpcre2-*.dll', 'libpng16-*.dll',
            'libstdc++-*.dll', 'libunistring-*.dll', 'libwinpthread-*.dll',
            'libzstd.dll', 'zlib1.dll',
            'qt.conf', 'Qt6Core.dll', 'Qt6Gui.dll', 'Qt6Widgets.dll',
            'digraph.exe', 'README.html',
            ]

if os.path.exists(destination_path):
    raise Exception("Destination path must be empty")

os.makedirs(destination_path)

for g in needed_output_files:
    matches = glob.glob(os.path.join(source_path, g))
    if len(matches) == 0:
        raise Exception("No match: " + g)
    for m in matches:
        dirname = os.path.relpath(os.path.dirname(m), source_path)
        destdir = os.path.join(destination_path, dirname)
        if not os.path.exists(destdir):
            os.makedirs(destdir)
        if os.path.isdir(m):
            shutil.copytree(m, os.path.join(destdir, os.path.basename(m)))
        else:
            shutil.copy(m, destdir)
