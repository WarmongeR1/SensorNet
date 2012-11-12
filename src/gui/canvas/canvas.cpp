#include "canvas.h"
#include "ui_canvas.h"
#include "Options.h"

Canvas::Canvas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Canvas),
    m_canvas(FIELD_X, FIELD_Y, QImage::Format_RGB32)
{
    ui->setupUi(this);

    // init
    m_canvas.fill(qRgb(255, 255, 255));
}
//------------------------------------------------------------------------------
Canvas::~Canvas()
{
    delete ui;
}
//------------------------------------------------------------------------------
void Canvas::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawImage(
                QRect(0
                      , 0
                      , m_canvas.size().width()
                      , m_canvas.size().height())
                , m_canvas);
}
//------------------------------------------------------------------------------
void Canvas::setImage(QImage *ShowedImage)
{
    m_canvas = *ShowedImage;
    update();
}
//------------------------------------------------------------------------------
void Canvas::setImage(QString* PathToImage)
{
    m_canvas = QImage(*PathToImage);
    update();
}
//------------------------------------------------------------------------------
