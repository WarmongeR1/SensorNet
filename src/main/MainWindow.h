#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"

class SensorNet : public QMainWindow
{
	Q_OBJECT

public:
	SensorNet(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SensorNet();

private:
	Ui::SensorNetClass ui;
};

#endif // MAINWINDOW_H
