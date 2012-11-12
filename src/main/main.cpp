#include "MainWindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SensorNet w;
	w.show();
	return a.exec();
}
