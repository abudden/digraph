TEMPLATE = app
TARGET = digraph

INCLUDEPATH += .
INCLUDEPATH += ./inc

# For debug output:
#CONFIG += console

win32 {
	LIBS += -luser32
	#RC_FILE = ./res/icon.rc
	PYTHON = python
	INCLUDEPATH += lib/windows
	QMAKE_CXXFLAGS += -isystem $$PWD/lib/windows/

	SOURCES += src/windows_specific.cpp
	SOURCES += lib/windows/sendkeys.cpp
	HEADERS += lib/windows/sendkeys.h

}

CONFIG(dockerwin) {
	# Cross-compiling for Windows via MXE and docker
	PYTHON = python3

	CONFIG(winstatic) {
		OBJECTS_DIR = generated_files/winstatic
		MOC_DIR = generated_files/winstatic
		RCC_DIR = generated_files/winstatic
		UI_DIR = generated_files/winstatic
		DESTDIR = output/winstatic

		CONFIG -= import_plugins
		DEFINES += IMPORT_PLUGINS

		QTPLUGIN += qwindows
	}
	CONFIG(winshared) {
		OBJECTS_DIR = generated_files/winshared
		MOC_DIR = generated_files/winshared
		RCC_DIR = generated_files/winshared
		UI_DIR = generated_files/winshared
		DESTDIR = output/winshared
	}
}
else {
	win32 {
		# Compiling directly on Windows

		console {
			OBJECTS_DIR = generated_files/console
			MOC_DIR = generated_files/console
			RCC_DIR = generated_files/console
			UI_DIR = generated_files/console
			DESTDIR = output/winconsole
		}
		else {
			OBJECTS_DIR = generated_files/release
			MOC_DIR = generated_files/release
			UI_DIR = generated_files/release
			RCC_DIR = generated_files/release
			DESTDIR = output/winrelease
		}
	}
	linux {
		PYTHON = python3
		OBJECTS_DIR = generated_files/linux
		MOC_DIR = generated_files/linux
		UI_DIR = generated_files/linux
		RCC_DIR = generated_files/linux
		DESTDIR = output/linux
	}
}

QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]
QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtNetwork

getchangeset.commands = $$PYTHON get_changeset.py

QMAKE_EXTRA_TARGETS += getchangeset
PRE_TARGETDEPS += getchangeset

CONFIG -= debug_and_release debug_and_release_target
CONFIG += release

QT += widgets
QT += network

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
SOURCES += \
	src/changeset.cpp \
	src/digraph.cpp \
	src/main.cpp
HEADERS += \
	inc/digraph.h \
	inc/osspecific.h
