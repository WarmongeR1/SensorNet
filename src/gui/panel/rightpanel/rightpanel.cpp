#include "rightpanel.h"
#include "ui_rightpanel.h"
#include <QFileDialog>
#include "filecommon.h"

#include "src/debug/debughelper.h"

RightPanel::RightPanel(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::RightPanel)
{
    ui->setupUi(this);
    createConnects();

    ui->pBGenerateImage->setEnabled(false);
    ui->pBShowImages->setEnabled(false);

    //
    ui->LETestDir->setText("/home/files/Develop/git/HTM/SensorNet-build/build/bin/testdata");
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
    connect(ui->pBBrowseTestData, SIGNAL(clicked()), SLOT(browseTestDir()));
    connect(ui->pBTestRun, SIGNAL(clicked()), SLOT(testRun()));
}
//------------------------------------------------------------------------------
void RightPanel::runNet()
{
    ui->pBGenerateImage->setEnabled(true);
    ui->pBShowImages->setEnabled(true);
    emit SIGNAL_RunNet(ui->spinBox->value());
}
//------------------------------------------------------------------------------
void RightPanel::browseTestDir()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this
                                                          , tr("Select input folder")
                                                          , ""
                                                          , options);
    if (!directory.isEmpty())
        ui->LETestDir->setText(directory);
}
//------------------------------------------------------------------------------
void RightPanel::testRun()
{
    emit SIGNAL_TestRun(ui->LETestDir->text());
}
//------------------------------------------------------------------------------
void RightPanel::setResult()
{
//    myDebug() << "set result";
//    QString text;
//    text = getResult();
    ui->textBrResult->setPlainText(getResult());
}
//------------------------------------------------------------------------------
