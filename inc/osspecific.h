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
