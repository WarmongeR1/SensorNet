#include "rightpanel.h"
#include "ui_rightpanel.h"

RightPanel::RightPanel(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::RightPanel)
{
    ui->setupUi(this);
    createConnects();

    ui->pBGenerateImage->setEnabled(false);
    ui->pBShowImages->setEnabled(false);
}
//------------------------------------------------------------------------------
RightPanel::~RightPanel()
{
    delete ui;
}
//------------------------------------------------------------------------------
void RightPanel::createConnects()
{
    connect(ui->pBGenerateImage, SIGNAL(clicked()), SIGNAL(SIGNAL_GenerateImages()));
    connect(ui->pBRunNet, SIGNAL(clicked()), SLOT(runNet()));
    connect(ui->pBShowImages, SIGNAL(clicked()), SIGNAL(SIGNAL_ShowImages()));
}
//------------------------------------------------------------------------------
void RightPanel::runNet()
{
    ui->pBGenerateImage->setEnabled(true);
    ui->pBShowImages->setEnabled(true);
    emit SIGNAL_RunNet(ui->spinBox->value());
}
//------------------------------------------------------------------------------
