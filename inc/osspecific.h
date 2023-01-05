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
#ifndef OSSPECIFIC_H
#define OSSPECIFIC_H

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QString>
#include <QPoint>
#include <QMap>
#include <QObject>

class OSSpecific : public QObject
{
	public:
		OSSpecific();
		// Called on start-up to make sure the currently-active
		// window is noted
		void getLastWindow();
		// Check whether the last window was recorded correctly
		bool hasLastWindow();
		// Find the centre of the last active window so we can
		// position the digraph window in the middle of that window
		QPoint getLastWindowCentre();
		// Bring the last window to the front (before sending paste)
		void bringLastWindowToFront();
		// Send the paste keyboard shortcut to the last window
		void pasteToLastWindow();
		// Copy to clipboard via system API calls or return false
		// to try QClipboard
		bool copyToClipboard(QString text);

	private:
		void clearLastWindow();

		bool has_last_window = false;
		QMap<QString, QString> windowPastes;

#ifdef Q_OS_WIN
		HWND last_window;
#endif
		QPoint last_window_centre;
};

#endif
