#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

#include "osspecific.h"
#include "sendkeys.h"

OSSpecific::OSSpecific()
{
	QSettings settings(QCoreApplication::applicationDirPath() + "/config/windows.ini",
			QSettings::IniFormat);

	settings.beginGroup("PasteCommands");
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
