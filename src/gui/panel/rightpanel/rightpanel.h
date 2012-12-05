#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include <QDockWidget>
#include <QDesktopWidget> // moved gui to center

namespace Ui {
class RightPanel;
}

class RightPanel : public QDockWidget
{
    Q_OBJECT

public:
    explicit RightPanel(QWidget *parent = 0);
    ~RightPanel();

    void setResult();

    QString getTrainFolder();

signals:
//    void SIGNAL_GetNextImage();
    void SIGNAL_ShowImages();
    void SIGNAL_RunNet(int);
    void SIGNAL_GenerateImages();
    void SIGNAL_TestRun(QString);

private slots:
    void runNet();

    void testRun();

    void browseTestDir();
    void browseTrainDir();
private:
    Ui::RightPanel *ui;

    /**
     * @brief createConnects
     */
    void createConnects();

    QString trainFolder;
};

#endif // RIGHTPANEL_H
