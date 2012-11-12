#ifndef NET_H
#define NET_H

#include "DataSample.h"
#include "NetParams.h"
#include "Sensor.h"

#include <QList>
#include <QVector>

class Net : public QObject
{
    Q_OBJECT

public:
    Net(NetParams netParams);

    void SetInput(const DataSample & dataSample);
    void Train(const QList<DataSample> & trainData);
    void Step(const bool train);

    void Operate(const QList<DataSample> & testData);

    //const NetParams & GetParams() const;
    void FormSensorImages();

signals:
    void SIGNAL_ShowImage(QString*);

private:
    void SupressNeighbours(Sensor * sensor);

    NetParams netParams;

    DataSample field;
    QList<Sensor> sensors;
};

#endif
