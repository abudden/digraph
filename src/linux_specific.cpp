/*
 * Copyright (c) 2023 A. S. Budden
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <QCoreApplication>
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QWidget>

#include "osspecific.h"

OSSpecific::OSSpecific()
{
	QSettings settings(QCoreApplication::applicationDirPath() + "/config/settings.ini",
			QSettings::IniFormat);

	settings.beginGroup("LinuxPasteCommands");
	QStringList keys = settings.childKeys();

	windowPastes["default"] = "^v";

	for (auto & key: keys) {
		windowPastes[key] = settings.value(key).toString();
	}

	settings.endGroup();
}

bool OSSpecific::hasLastWindow()
{
	return has_last_window;
}

bool OSSpecific::copyToClipboard(QString text)
{
	// Rely on the OS-independent code in digraph.cpp
	(void) text;
	return false;
}

void OSSpecific::clearLastWindow()
{
	has_last_window = false;
	last_window_centre = QPoint(0,0);
}

void OSSpecific::getLastWindow()
{
	// TODO - currently does nothing
}

void OSSpecific::bringLastWindowToFront()
{
	if (has_last_window) {
		// TODO: currently does nothing
	}
}

void OSSpecific::pasteToLastWindow()
{
	// TODO: Currently does nothing
}

QPoint OSSpecific::getLastWindowCentre()
{
	return last_window_centre;
}
