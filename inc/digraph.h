#ifndef DIGRAPHWINDOW_H
#define DIGRAPHWINDOW_H

#include <QWidget>

#include "osspecific.h"

#include "ui_digraph.h"

class DigraphWindow : public QDialog
{
	Q_OBJECT

	public:
		explicit DigraphWindow(QDialog *parent = 0);

	protected:
		bool eventFilter(QObject *object, QEvent *e);

	public slots:
		void prepareDigraph();

	private:
		void readEntries();
		void copyToClipboard(QString result);
		void showVersion();
		void done(bool clipboardFilled=false);
		void sendPasteAndClose();
		void quit();

		Ui_dlgDigraph ui;

		QMap<QString, QString> entries;

		OSSpecific os;
};

#endif
