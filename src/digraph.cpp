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
#include <QtWidgets>
#include <QDebug>
#include <QtNetwork>

#include "digraph.h"

extern const char * changeset;
extern const char * version;
extern const char * builddate;

DigraphWindow::DigraphWindow(bool useBigUi, QDialog *parent) :
	QDialog(parent)
{
	os.getLastWindow();

	// If specified on the command-line, then show the big window
	if (useBigUi) {
		showBigUi = true;
	}
	else {
		// Otherwise, check the setting in the ini file
		showBigUi = getSetting("UI", "LargeWindow");
	}

	setupUi();

	qApp->installEventFilter(this);

	activateWindow();
	QTimer::singleShot(100, this, QDialog::activateWindow);

	readEntries();

	if (showBigUi) {
		showExamples();
	}
}

void DigraphWindow::setupUi()
{
	QSize windowSize;

	QVBoxLayout *mainLayout = new QVBoxLayout();
	edtDigraph = new QLineEdit();

	if (showBigUi) {
		windowSize = QSize(209, 216);

		QHBoxLayout *lyt1 = new QHBoxLayout();
		lyt1->addWidget(new QLabel("Enter Digraph:"));
		lyt1->addWidget(edtDigraph);
		mainLayout->addLayout(lyt1);

		lblExampleHeader = new QLabel();
		mainLayout->addWidget(lblExampleHeader);

		QHBoxLayout *exampleLyt = new QHBoxLayout();
		for (int i=0;i<(EXAMPLE_LABEL_COUNT/2);i++) {
			lblExamples[2*i] = new QLabel("ab : ");
			lblExamples[2*i]->setAlignment(
					Qt::AlignTrailing | Qt::AlignRight | Qt::AlignTop);
			exampleLyt->addWidget(lblExamples[2*i]);

			lblExamples[(2*i)+1] = new QLabel("a");
			lblExamples[(2*i)+1]->setAlignment(
					Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);
			exampleLyt->addWidget(lblExamples[(2*i)+1]);

			//exampleLyt->addSpacerItem(new QSpacerItem(40, 20));
		}

		mainLayout->addLayout(exampleLyt);

		mainLayout->addStretch(1);
	}
	else {
		windowSize = QSize(94, 38);
		mainLayout->addWidget(edtDigraph);
	}

	setLayout(mainLayout);

	connect(edtDigraph, &QLineEdit::textEdited, this, &DigraphWindow::prepareDigraph);

	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
	setWindowFlags(windowFlags() &(~Qt::WindowMaximizeButtonHint));

	setFixedSize(windowSize);

	QPoint lwc = os.getLastWindowCentre();
	if ( ! lwc.isNull()) {
		QSize s = size();
		lwc.setX(lwc.x() - (s.width()/2));
		lwc.setY(lwc.y() - (s.height()/2));
		move(lwc);
	}
}

void DigraphWindow::showExamples(QString prefix)
{
	QHash<QString, QString> filtered_entries;
	static const QStringList defaultExamples = {
		"a!", "e'", "i^", "n?",
		"2S", "3s", "-N", "->", "<=", "=<",
		">=", "00", "Co", "TM", "/\\", "-:", "+-",
		"RT", "!=", "?=",
		"1'", "/-", "/=", "SE", ".:", "OK", "XX",
		"a*", "b*", "l*", "L*", "m*", "W*",
		"-!", "-v", "<>", "UD", "12", "14", "DG",
	};

	for (int i=0 ; i < EXAMPLE_LABEL_COUNT; i++) {
		lblExamples[i]->setText("");
	}

	int label = 0;
	QStringList keys;
	if (prefix.length() == 0) {
		// Standard examples
		keys = defaultExamples;
		lblExampleHeader->setText("Example Digraphs:");
	}
	else {
		lblExampleHeader->setText("Available Digraphs:");
		QString p = prefix.left(1);
		for (auto & key : entries.keys()) {
			if (key.left(1) == p) {
				keys << key;
			}
		}
	}
	for (auto & key : keys) {
		if (entries.contains(key)) {
			lblExamples[label]->setText(
					lblExamples[label]->text() +
					key + " : \n");
			lblExamples[label+1]->setText(
					lblExamples[label+1]->text() +
					entries[key] + "\n");
			label += 2;
			label %= EXAMPLE_LABEL_COUNT;
		}
	}
}

void DigraphWindow::copyToClipboard(QString result)
{
	if ( ! os.copyToClipboard(result)) {
		qDebug() << "Copying to clipboard:" << result;
		QClipboard *clipboard = QGuiApplication::clipboard();
		qDebug() << "Existing content (to be overwritten):" << clipboard->text(QClipboard::Clipboard);
		clipboard->setText(result, QClipboard::Clipboard);
		qDebug() << "Clipboard contents:" << clipboard->text(QClipboard::Clipboard);
	}
}

void DigraphWindow::showVersion()
{
	QString versionInfo = "Version: ";
	versionInfo += version;
	if (QString(changeset).endsWith("+")) {
		versionInfo += " (modified)";
	}
	versionInfo += "\n";
	versionInfo += builddate;
	QMessageBox msgBox(
			QMessageBox::Information,
			"Digraph",
			versionInfo
			);
	QAbstractButton *pButtonCopy = msgBox.addButton("Copy to Clipboard", QMessageBox::YesRole);
	msgBox.addButton("Exit", QMessageBox::NoRole);
	msgBox.setWindowFlags(Qt::WindowStaysOnTopHint);
	msgBox.exec();
	if (msgBox.clickedButton() == pButtonCopy) {
		copyToClipboard(versionInfo);
	}
}

void DigraphWindow::prepareDigraph()
{
	QString enteredString;
	enteredString = edtDigraph->text();
	if (showBigUi) {
		showExamples(enteredString);
	}

	if (enteredString.length() >= 2) {
		QString digraph = enteredString.left(2);
		QString reversedDigraph = digraph.right(1) + digraph.left(1);
		if (digraph == "VE") {
			showVersion();
			done(false);
		}
		else if (entries.contains(digraph)) {
			QString result = entries[digraph];
			// If I call copyToClipboard just once, it works in Word, Outlook
			// and some others, but doesn't work in notepad or web browsers.
			// https://stackoverflow.com/questions/74687828/qclipboard-doesnt-work-if-last-application-was-notepad-or-web-browser
			// Calling it 8 times seems to fix the issue, but I don't like it.
			for (int i=0;i<8;i++) {
				copyToClipboard(result);
				QThread::msleep(5);
			}
			done(true);
		}
		else if (entries.contains(reversedDigraph)) {
			QString result = entries[reversedDigraph];
			// If I call copyToClipboard just once, it works in Word, Outlook
			// and some others, but doesn't work in notepad or web browsers.
			// https://stackoverflow.com/questions/74687828/qclipboard-doesnt-work-if-last-application-was-notepad-or-web-browser
			// Calling it 8 times seems to fix the issue, but I don't like it.
			for (int i=0;i<8;i++) {
				copyToClipboard(result);
				QThread::msleep(5);
			}
			done(true);
		}
		else {
			QMessageBox::critical(this, "Error",
					QString("Digraph not found: %1").arg(digraph));
			done(false);
		}
	}
}

bool DigraphWindow::eventFilter(QObject *object, QEvent *e)
{
	if (e->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
		int key = keyEvent->key();

		if (key == Qt::Key_Escape) {
			done();
			return true;
		}
	}

	return QDialog::eventFilter(object, e);
}

void DigraphWindow::done(bool clipboardFilled)
{
	edtDigraph->setText("");

	if (clipboardFilled && os.hasLastWindow()) {
		os.bringLastWindowToFront();
		hide();
		QTimer::singleShot(150, this, &sendPasteAndClose);
	}
	else {
		quit();
	}
}

void DigraphWindow::quit()
{
	close();
	QCoreApplication::quit();
}

void DigraphWindow::sendPasteAndClose()
{
	os.pasteToLastWindow();
	qDebug() << "Done; closing";
	quit();
}

void DigraphWindow::readEntries()
{
	QFile f(QCoreApplication::applicationDirPath() + "/config/entries.txt");
	if ( ! f.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(0, "Error reading config file", f.errorString());
	}

	QTextStream in(&f);

	while ( ! in.atEnd()) {
		QString line = in.readLine();
		QStringList fields = line.trimmed().split(" ");

		if ((fields.size() >= 2) && (fields[0].length() == 2)) {
			bool ok;
			int keycode = fields[1].toInt(&ok, 0);
			if ( ! ok) {
				continue;
			}
			entries[fields[0]] = QString(QChar(keycode));
		}
	}

	f.close();
}

bool DigraphWindow::getSetting(QString group, QString setting, bool defaultSetting)
{
	bool result = defaultSetting;
	QSettings settings(QCoreApplication::applicationDirPath() + "/config/settings.ini",
			QSettings::IniFormat);

	settings.beginGroup(group);

	if (settings.contains(setting)) {
		result = (settings.value(setting).toString().toLower() == "true");
	}

	settings.endGroup();

	return result;
}
