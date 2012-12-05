#include "canvas.h"
#include "ui_canvas.h"
#include "Options.h"
#include "src/debug/debughelper.h"
#include "src/common/filecommon.h"


#include <QDir>

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
    //    painter.drawImage(
    //                QRect(rect.width() / 2
    //                      , rect.height() / 2
    //                      , m_canvas.size().width()
    //                      , m_canvas.size().height())
    //                , m_canvas);

    painter.drawImage(0,0, m_canvas);
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
void Canvas::showImages()
{
//    myDebug() << "show images";

    QDir dir;
    QString t_path = dir.currentPath() + "/netinternals/";
    QStringList m_listFiles = recursiveFind(t_path);

    int colCount = 800 / 64;

    int col = 0;
    int row = 0;

    QImage canvas(800, 600,  QImage::Format_RGB32);
    canvas.fill(qRgb(255, 255, 255));

    QPainter painter(&canvas);
    for (int i = 0; i < m_listFiles.size(); i++)
    {

        QImage image;
        image.load(m_listFiles.at(i));
        image = image.scaledToHeight(64);

//         image = jpgImage->scaled(inputWidth.toUInt(), inputHeight.toUint,Qt::KeepAspectRatio);
//        image.scaled(64, 64,  Qt::IgnoreAspectRatio);
//        myDebug() << image.height();

        painter.drawImage(image.height()*col, image.height()*row, image);

        col++;
        if (col >= colCount)
        {
            col = 0;
            row++;
        }

    }

    painter.end();
    setImage(&canvas);
}
//------------------------------------------------------------------------------
