#ifndef NET_H
#define NET_H

#include "DataSample.h"
#include "NetParams.h"
#include "Sensor.h"

#include <QList>
#include <QVector>


struct Result
{
    Result()
    {
        for(int i = 0; i < TYPES_N; i++)
            result[i] = 0.0;
    }

    int type;
    double result[TYPES_N];
};


class Net : public QObject
{
    Q_OBJECT

public:
    Net(NetParams netParams, bool silentMode);

    void SetInput(const DataSample & dataSample);
    void Train(const QList<DataSample> & trainData, int cyclesToRepeat);
    void Step(const bool train, int sampleType);

    void Operate(const QList<DataSample> & testData);

    //const NetParams & GetParams() const;
    void FormSensorImages();

    void PrintResults();
    void FindResult();
    double GetResult() const;

    const NetParams & GetParams() const;

public slots:
//    QImage FormNextSensorImage();

signals:
    void SIGNAL_ShowImage(QString*);
    void SIGNAL_ShowImage(QImage*);

    void TrainProgress(int value);
    void TestProgress(int value);

private:
    void SupressNeighbours(Sensor * sensor);

    NetParams netParams;

    int m_CountCurSensor;

    DataSample field;
    QList<Sensor> sensors;

    bool silentMode;

    QList<Result> results;
    QList<Result> saccadeResults;

    double result;
};

#endif
