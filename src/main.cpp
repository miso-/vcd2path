#include "Vcd2path.h"

#include <QtGui/QApplication>
#include <QDataStream>
#include <QSettings>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setApplicationName("vcd2path");

	Vcd2path mainWindow;
	mainWindow.show();
	return app.exec();
}
