#include <QtWidgets>
#include <QDebug>
#include <QtNetwork>

#include "digraph.h"

extern const char * changeset;
extern const char * version;
extern const char * builddate;

DigraphWindow::DigraphWindow(QDialog *parent) :
	QDialog(parent)
{
	os.getLastWindow();

	ui.setupUi(this);

	QPoint lwc = os.getLastWindowCentre();
	if ( ! lwc.isNull()) {
		QSize s = size();
		lwc.setX(lwc.x() - (s.width()/2));
		lwc.setY(lwc.y() - (s.height()/2));
		move(lwc);
	}

	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
	connect(ui.edtDigraph, &QLineEdit::textChanged, this, &DigraphWindow::prepareDigraph);

	qApp->installEventFilter(this);

	activateWindow();
	QTimer::singleShot(100, this, QDialog::activateWindow);

	readEntries();
}

void DigraphWindow::copyToClipboard(QString result)
{
	qDebug() << "Copying to clipboard:" << result;
	QClipboard *clipboard = QGuiApplication::clipboard();
	qDebug() << "Existing content (to be overwritten):" << clipboard->text(QClipboard::Clipboard);
	clipboard->setText(result, QClipboard::Clipboard);
	qDebug() << "Clipboard contents:" << clipboard->text(QClipboard::Clipboard);
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
	msgBox.exec();
	if (msgBox.clickedButton() == pButtonCopy) {
		copyToClipboard(versionInfo);
	}
}

void DigraphWindow::prepareDigraph()
{
	QString enteredString = ui.edtDigraph->text();

	if (enteredString.length() >= 2) {
		QString digraph = enteredString.left(2);
		QString reversedDigraph = digraph.right(1) + digraph.left(1);
		if (digraph == "VE") {
			showVersion();
			done(false);
		}
		else if (entries.contains(digraph)) {
			QString result = entries[digraph];
			copyToClipboard(result);
			done(true);
		}
		else if (entries.contains(reversedDigraph)) {
			QString result = entries[reversedDigraph];
			copyToClipboard(result);
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
	ui.edtDigraph->setText("");

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
