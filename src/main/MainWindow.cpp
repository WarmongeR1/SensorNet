#include "MainWindow.h"

#include "Net.h"
#include "ImageDataCreator.h"

SensorNet::SensorNet(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	ImageDataCreator imageDataCreator;
	imageDataCreator.CreateImageData("traindata");

	NetParamsInitializer params;
	params.ActivePermanence = 0.4;
	params.ActiveSensorPercent = 0.1;
	params.ActiveSynapseStartPercent = 0.5;
	params.DeadPermanenceIncrease = 0.01;
	params.DeadSensorBoost = 10.0;
	params.PermanenceDecrease = 0.015;
	params.PermanenceIncrease = 0.015;
	params.SensorBoost = 1.2;
	params.SensorInhibitionPercent = 0.8;
	params.TrivialPatternTreshold = 5;

	NetParams netParams(params);

	Net net(netParams);

	for(uint i = 0; i < 20; i++)
		net.Train(imageDataCreator.GetData());

	net.FormSensorImages();

	net.Operate(imageDataCreator.GetData());
}

SensorNet::~SensorNet()
{

}
