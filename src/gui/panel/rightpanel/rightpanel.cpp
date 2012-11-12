#include "rightpanel.h"
#include "ui_rightpanel.h"

RightPanel::RightPanel(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::RightPanel)
{
    ui->setupUi(this);
    createConnects();
}
//------------------------------------------------------------------------------
RightPanel::~RightPanel()
{
    delete ui;
}
//------------------------------------------------------------------------------
void RightPanel::createConnects()
{

}
//------------------------------------------------------------------------------
