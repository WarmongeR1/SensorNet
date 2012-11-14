#include "canvas.h"
#include "ui_canvas.h"
#include "Options.h"
#include "src/debug/debughelper.h"

Canvas::Canvas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Canvas),
    m_canvas(FIELD_X * 10, FIELD_Y * 10, QImage::Format_RGB32)
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
    QRect rect = this->rect();
    QPainter painter(this);
    painter.drawImage(
                QRect(rect.width() / 2
                      , rect.height() / 2
                      , m_canvas.size().width()
                      , m_canvas.size().height())
                , m_canvas);
    painter.end();
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
