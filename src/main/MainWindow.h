#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"

#include "src/gui/panel/rightpanel/rightpanel.h"
#include "src/gui/canvas/canvas.h"
#include "ImageDataCreator.h"

class Net;
class ImageDataCreator;

class SensorNet : public QMainWindow
{
    Q_OBJECT

public:
    SensorNet(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~SensorNet();

private slots:
    void showImages();

    void generateImages();
    void runNet(int);

private:
    Ui::SensorNetClass ui;

    RightPanel * GUI_RightPanel;
    Canvas * GUI_Canvas;

    ImageDataCreator imageDataCreator;

    Net *m_Net;
    int t_count;

    /**
     * @brief createConnects
     *
     */
    void createConnects();

    /**
     * @brief createGui
     * Create central widget and panels and other gui
     */
    void createGui();
};

#endif // MAINWINDOW_H
