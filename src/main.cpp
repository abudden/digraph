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
#include "digraph.h"

#ifdef IMPORT_PLUGINS
#include <QtPlugin>
#ifdef Q_OS_WIN
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif
#endif

extern const char * version;
int main(int argc, char *argv[])
{
	// Creates an instance of QApplication
	QApplication a(argc, argv);

	QCommandLineParser parser;
	QApplication::setApplicationName("digraph");
	QApplication::setApplicationVersion(QString(version));
	parser.setApplicationDescription("Digraph Tool");
	parser.addHelpOption();
	parser.addVersionOption();

	QCommandLineOption showBigUIOption(QStringList() << "l" << "large",
			"Show large UI window with example digraphs");
	parser.addOption(showBigUIOption);

	parser.process(a);

	// This is our MainWidget class containing our GUI and functionality
	DigraphWindow w(parser.isSet(showBigUIOption));
	w.show(); // Show main window

	// run the application and return execs() return value/code
	return a.exec();
}
