

#include "MainWindow.h"
#include "Net.h"
#include "src/debug/debughelper.h"
#include "filecommon.h"


SensorNet::SensorNet(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);


    //    ImageDataCreator imageDataCreator;
    imageDataCreator.CreateImageData("traindata");

    NetParamsInitializer params;
    params.ActivePermanence = 0.4;
    params.ActiveSensorPercent = 0.1;
    params.ActiveSynapseStartPercent = 0.5;
    params.DeadPermanenceIncrease = 0.01;
    params.DeadSensorBoost = 10.0;
    params.PermanenceDecrease = 0.015;
    params.PermanenceIncrease = 0.015;
    params.SensorBoost = 1.001;
    params.SensorInhibitionPercent = 0.8;
    params.TrivialPatternTreshold = 5;

    NetParams netParams(params);
    m_Net = new Net(netParams, false);
    //    m_Net->Train(imageDataCreator.GetData(), 100);
    //    m_Net->FormSensorImages();

//    myDebug() << m_Net;

    t_count = 0;
    createGui();
    createConnects();

    //    imageDataCreator.CreateImageData("testdata");
    //    m_Net->Operate(imageDataCreator.GetData());

    //    m_Net->PrintResults();

    //-----------
    // gui



//    myDebug() << getResult();


    // moved to center desktop
    QRect rect = QApplication::desktop()->availableGeometry(this);
    this->move(rect.width() / 2 - this->width() / 2,
               rect.height() / 2 - this->height() / 2);
    // maximized
    //    this->showMaximized();

    // /gui
    // ----------
}
//------------------------------------------------------------------------------
SensorNet::~SensorNet()
{
    GUI_RightPanel->deleteLater();
    GUI_Canvas->deleteLater();
    delete this;
}
//------------------------------------------------------------------------------
void SensorNet::createConnects()
{
    connect(ui.actionMenuQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    //    connect(m_Net, SIGNAL(SIGNAL_ShowImage(QString*)), GUI_Canvas,
    //            SLOT(setImage(QString*)));
    //    connect(m_Net, SIGNAL(SIGNAL_ShowImage(QImage*)), GUI_Canvas, SLOT(setImage(QImage*)));
    //    connect(GUI_RightPanel, SIGNAL(SIGNAL_GetNextImage()), m_Net, SLOT(FormNextSensorImage()));
    connect(GUI_RightPanel, SIGNAL(SIGNAL_ShowImages()), SLOT(showImages()));
    connect(GUI_RightPanel, SIGNAL(SIGNAL_RunNet(int)), SLOT(runNet(int)));
    connect(GUI_RightPanel, SIGNAL(SIGNAL_GenerateImages()), SLOT(generateImages()));

    connect(GUI_RightPanel, SIGNAL(SIGNAL_TestRun(QString)), SLOT(testRun(QString)));
}
//------------------------------------------------------------------------------
void SensorNet::createGui()
{
    // create central widget
    GUI_Canvas = new Canvas(this);
    setCentralWidget(GUI_Canvas);

    // create panels
    GUI_RightPanel = new RightPanel(this);
    GUI_RightPanel->setMinimumWidth(250);
    addDockWidget(Qt::RightDockWidgetArea, GUI_RightPanel);
}
//------------------------------------------------------------------------------
void SensorNet::runNet(int m_for)
{

    //    t_count++;
    //    for(uint i = 0; i < 20; i++)
    //    {
    //        m_Net->Train(imageDataCreator.GetData());
    //        m_Net->FormSensorImages();
    //        myDebug() << "click" << t_count;
    //    }



    //    ImageDataCreator imageDataCreator;
    imageDataCreator.CreateImageData("traindata");

    NetParamsInitializer params;
    params.ActivePermanence = 0.4;
    params.ActiveSensorPercent = 0.1;
    params.ActiveSynapseStartPercent = 0.5;
    params.DeadPermanenceIncrease = 0.01;
    params.DeadSensorBoost = 10.0;
    params.PermanenceDecrease = 0.015;
    params.PermanenceIncrease = 0.015;
    params.SensorBoost = 1.001;
    params.SensorInhibitionPercent = 0.8;
    params.TrivialPatternTreshold = 5;

    NetParams netParams(params);
    m_Net = new Net(netParams, false);

    connect(m_Net, SIGNAL(SIGNAL_ShowImages()), SLOT(showImages()));
    //    m_Net->Train(imageDataCreator.GetData(), 100);


    //    myDebug() << m_Net;


    //    m_Net = new Net(netParams, false);
    m_Net->Train(imageDataCreator.GetData(), m_for, true);
    //    m_Net->Train(imageDataCreator.GetData(), m_for);
    //    m_Net->FormSensorImages();
}
//------------------------------------------------------------------------------
void SensorNet::generateImages()
{
    //    myDebug() << "generate images";
    //    imageDataCreator.CreateImageData("testdata");
    //    m_Net->Operate(imageDataCreator.GetData());



    //        ImageDataCreator imageDataCreator;
    //    imageDataCreator.CreateImageData("traindata");

    //    NetParamsInitializer params;
    //    params.ActivePermanence = 0.4;
    //    params.ActiveSensorPercent = 0.1;
    //    params.ActiveSynapseStartPercent = 0.5;
    //    params.DeadPermanenceIncrease = 0.01;
    //    params.DeadSensorBoost = 10.0;
    //    params.PermanenceDecrease = 0.015;
    //    params.PermanenceIncrease = 0.015;
    //    params.SensorBoost = 1.001;
    //    params.SensorInhibitionPercent = 0.8;
    //    params.TrivialPatternTreshold = 5;

    //    NetParams netParams(params);
    //    m_Net = new Net(netParams, false);
    m_Net->FormSensorImages();
    //    m_Net->PrintResults();
}
//------------------------------------------------------------------------------
void SensorNet::showImages()
{
    //        myDebug() << " show images";
    GUI_Canvas->showImages();
}
//------------------------------------------------------------------------------
void SensorNet::testRun(QString path)
{
    imageDataCreator.CreateImageData(path);
    m_Net->Operate(imageDataCreator.GetData());

    m_Net->PrintResults();
    GUI_RightPanel->setResult();
}
//------------------------------------------------------------------------------
