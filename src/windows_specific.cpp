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
#include "sendkeys.h"

OSSpecific::OSSpecific()
{
	QSettings settings(QCoreApplication::applicationDirPath() + "/config/settings.ini",
			QSettings::IniFormat);

	settings.beginGroup("WindowsPasteCommands");
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
	// Manual implementation of QClipboard::setText using the code from
	// https://bugreports.qt.io/secure/attachment/45584/clipboard.patch
	// Seems to work more reliably than the QClipboard version, which
	// uses OleSetClipboard
	QWidget *window = QApplication::activeWindow();
	bool result = (! window) ? false : OpenClipboard((HWND) window->winId());

	if (result)
	{
		const int length = text.length();
		const int size = (length + 1) *sizeof(wchar_t);
		HGLOBAL bufferHandle = ::GlobalAlloc(GMEM_MOVEABLE, size);
		{
			wchar_t *buffer = (wchar_t *) ::GlobalLock(bufferHandle);
			text.toWCharArray(buffer);
			buffer[length] = L'\0';
			GlobalUnlock(bufferHandle);
		}
		EmptyClipboard();
		HANDLE hr = SetClipboardData(CF_UNICODETEXT, bufferHandle);
		if (hr != bufferHandle)
		{
			qDebug() << "Failed to set data on clipboard";
			result = false;
		}
		CloseClipboard();
	}
	return result;
}

void OSSpecific::clearLastWindow()
{
	has_last_window = false;
	last_window_centre = QPoint(0,0);
}

void OSSpecific::getLastWindow()
{
	RECT last_window_rect;

	last_window = GetForegroundWindow();
	bool ok = GetWindowRect(last_window, &last_window_rect);

	qDebug() << "Got last window (" << ok << ") " << last_window;

	if (ok) {
		last_window_centre = QPoint((last_window_rect.left + last_window_rect.right) / 2,
				(last_window_rect.top + last_window_rect.bottom)/2);

		has_last_window = true;
	}
}

void OSSpecific::bringLastWindowToFront()
{
	if (has_last_window) {
		SetForegroundWindow(last_window);
	}
}

void OSSpecific::pasteToLastWindow()
{
	WCHAR buffer[255];
	qDebug() << "Last Window:" << last_window << " (" << has_last_window << ")";
	if ( ! has_last_window) {
		qDebug() << "No last window";
		return;
	}
	GetWindowTextW(last_window, buffer, 254);
	qDebug() << buffer;
	QString windowname = QString::fromWCharArray(buffer);

	qDebug() << "Pasting to last window " << windowname;

	CSendKeys sk;
	QString keys = windowPastes["default"];
	for (auto key : windowPastes.keys()) {
		if (key == "default") {
			continue;
		}
		if (windowname.contains(key)) {
			keys = windowPastes[key];
		}
	}
	qDebug() << "Sending " << keys;
	sk.SendKeys(keys.toStdWString().c_str(), true);

	clearLastWindow();
}

QPoint OSSpecific::getLastWindowCentre()
{
	return last_window_centre;
}
