#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QImage>
#include <QPainter>

namespace Ui {
class Canvas;
}

class Canvas : public QWidget
{
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = 0);
    ~Canvas();

    void setImage(QImage *showedImage);
    void setImage(QString *PathToImage);

protected:
    void paintEvent(QPaintEvent*);

private:
    Ui::Canvas *ui;

    QImage m_canvas;
};

#endif // CANVAS_H
