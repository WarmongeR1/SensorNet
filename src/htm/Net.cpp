#include "Net.h"
#include "Options.h"
#include "src/debug/debughelper.h"
#include "Random.h"


#include <QPair>
#include <QDir>

Net::Net(NetParams netParams_) : netParams(netParams_)
{
    for(uint i = 0; i < SENSOR_N; i++)
        sensors << Sensor(&netParams_, field);

    m_CountCurSensor = 0;
}
//------------------------------------------------------------------------------
void Net::SetInput(const DataSample & dataSample)
{
    for(uint i = 0; i < FIELD_Y; i++)
        for(uint j = 0; j < FIELD_X; j++)
            field.data[i][j] = dataSample.data[i][j];
}
//------------------------------------------------------------------------------
void Net::Train(const QList<DataSample> & trainData)
{
    for(uint n = 0; n < trainData.size(); n++)
    {
        int choice = Random::Range(trainData.size());
        SetInput(trainData[choice]);
        Step(true, trainData[choice].type);
    }
}
//------------------------------------------------------------------------------
void Net::Step(const bool train, int sampleType)
{
    QList<QPair<double, Sensor *> > sensorLadder;

    for(uint i = 0; i < SENSOR_N; i++)
    {
        sensors[i].UpdateSignal();
        sensors[i].SetState(Sensor::UNDEFINED);
        sensorLadder << QPair<double, Sensor *>(sensors[i].GetSignal(), &sensors[i]);
    }

    qSort(sensorLadder);

    const int WINNERS_N = SENSOR_N * netParams.ACTIVE_SENSOR_PERCENT;

    int winners = 0;
    for(uint i = 0; winners < WINNERS_N && i < SENSOR_N; i++)
    {
        if(sensorLadder[SENSOR_N - 1 - i].second->GetState() == Sensor::UNDEFINED)
        {
            sensorLadder[SENSOR_N - 1 - i].second->SetState(Sensor::ACTIVE);
            SupressNeighbours(sensorLadder[SENSOR_N - 1 - i].second);
        }
    }

    int passiveSensors = 0;
    for(uint i = 0; i < SENSOR_N; i++)
        if(sensorLadder[i].second->GetState() != Sensor::ACTIVE)
        {
            sensorLadder[i].second->SetState(Sensor::PASSIVE);
            passiveSensors++;
        }

    double realActiveSensorsPercent = (SENSOR_N - passiveSensors) / double(SENSOR_N);

    if(train)
        for(uint i = 0; i < SENSOR_N; i++)
            if(sensorLadder[i].second->GetState() == Sensor::ACTIVE)
            {
                sensorLadder[i].second->UpdateAsWinner(realActiveSensorsPercent);
                sensorLadder[i].second->IncreaseTypeCount(sampleType);
            }
            else
                sensorLadder[i].second->UpdateAsLooser();
    else
    {
        double resultTable[TYPES_N] = { 0.0 };

        double sum = 0.0;
        for(uint i = 0; i < SENSOR_N; i++)
            if(sensorLadder[i].second->GetState() == Sensor::ACTIVE)
            {
                for(int t = 0; t < TYPES_N; t++)
                    resultTable[t] += (sensorLadder[i].second->GetTypesTable())[t];
            }
        for(uint t = 0; t < TYPES_N; t++)
            sum += resultTable[t];

        Result result;
        result.type = sampleType;

        if(sum != 0.0)
            for(uint t = 0; t < TYPES_N; t++)
                result.result[t] = resultTable[t] / sum;
        else
            for(uint t = 0; t < TYPES_N; t++)
                result.result[t] = 0.0;

        results.push_back(result);
    }
}
//------------------------------------------------------------------------------
void Net::Operate(const QList<DataSample> & testData)
{
    results.clear();

    for(uint n = 0; n < testData.size(); n++)
    {
        SetInput(testData[n]);
        Step(false, testData[n].type);
    }

    PrintResults();
}
//------------------------------------------------------------------------------
void Net::SupressNeighbours(Sensor * sensor)
{
    QList<QPair<double, Sensor *> > neighbours;

    for(uint i = 0; i < SENSOR_N; i++)
        if(&sensors[i] != sensor)
        {
            int sensorX, sensorY, sensorR2;
            int tempX, tempY, tempR2;

            sensor->GetPosition(sensorX, sensorY, sensorR2);
            sensors[i].GetPosition(tempX, tempY, tempR2);

            int distance2 = (sensorX - tempX) * (sensorX - tempX) + (sensorY - tempY) * (sensorY - tempY);

            if(distance2 < sensorR2)
                neighbours << QPair<double, Sensor *>(sensors[i].GetSignal(), &sensors[i]);
        }

        qSort(neighbours);

        const int supressN = neighbours.size() * netParams.SENSOR_INHIBITION_PERCENT;
        int supressed = 0;

        for(uint i = 0; supressed < supressN && i < neighbours.size(); i++)
            if(neighbours[i].second->GetState() != Sensor::ACTIVE)
            {
                neighbours[i].second->SetState(Sensor::PASSIVE);
                supressed++;
            }
}
//------------------------------------------------------------------------------
void Net::FormSensorImages()
{
    QDir outputDir;
    outputDir.mkdir("netinternals");

    outputDir.cd("netinternals");

    for(uint n = 0; n < SENSOR_N; n++)
        sensors[n].FormImage(outputDir, n);
}
//------------------------------------------------------------------------------
void Net::PrintResults()
{
    QFile resultFile("result.txt");
    resultFile.open(QIODevice::WriteOnly);
    QTextStream out(&resultFile);

    for(uint i = 0; i < results.size(); i++)
    {
        int maxType = -1;
        double maxTypeValue = 0.0;
        for(int t = 0; t < TYPES_N; t++)
        {
            if(results[i].result[t] > maxTypeValue)
            {
                maxTypeValue = results[i].result[t];
                maxType = t;
            }
        }

        out << results[i].type << " - " << maxType << "(" << QString("%1").arg(results[i].result[maxType], 1, 'f', 2, QChar(0)) << ")" << " :";
        for(int t = 0; t < TYPES_N; t++)
            out << " " << t << "(" << QString("%1").arg(results[i].result[t], 1, 'f', 2, QChar(0)) << ")";
        out << "\r\n";
    }

    resultFile.close();
}
//------------------------------------------------------------------------------
