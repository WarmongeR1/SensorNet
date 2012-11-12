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

public slots:
    /**
     * @brief setImage
     * @param showedImage
     */
    void setImage(QImage *showedImage);
    /**
     * @brief setImage
     * @param PathToImage
     */
    void setImage(QString *PathToImage);

protected:
    /**
     * @brief paintEvent
     */
    void paintEvent(QPaintEvent*);

private:
    Ui::Canvas *ui;

    QImage m_canvas;
};

#endif // CANVAS_H
