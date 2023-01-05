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
#ifndef DIGRAPHWINDOW_H
#define DIGRAPHWINDOW_H

// Must be multiple of 2
#define EXAMPLE_LABEL_COUNT 8

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>

#include "osspecific.h"

class DigraphWindow : public QDialog
{
	Q_OBJECT

	public:
		explicit DigraphWindow(bool useBigUi=false, QDialog *parent = 0);

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
		void showExamples(QString prefix="");
		bool getSetting(QString group, QString setting, bool defaultSetting = false);
		bool showBigUi;
		void setupUi();

		QLineEdit *edtDigraph;
		QLabel *lblExampleHeader;
		QLabel *lblExamples[EXAMPLE_LABEL_COUNT];

		QMap<QString, QString> entries;

		OSSpecific os;
};

#endif
