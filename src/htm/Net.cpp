#include "Net.h"
#include "Options.h"
#include "src/debug/debughelper.h"
#include "Random.h"

#include <QProgressDialog>
#include <QProgressBar>
#include <QLabel>
#include <QApplication>

#include <QPair>
#include <QDir>
#include <QTextStream>

Net::Net(NetParams netParams_, bool silentMode_) : netParams(netParams_),
    silentMode(silentMode_)
{
    for(uint i = 0; i < SENSOR_N; i++)
        sensors << Sensor(&netParams_, field);
}
//------------------------------------------------------------------------------
void Net::SetInput(const DataSample & dataSample)
{
    for(uint i = 0; i < FIELD_Y; i++)
        for(uint j = 0; j < FIELD_X; j++)
            field.data[i][j] = dataSample.data[i][j];
}
//------------------------------------------------------------------------------
void Net::Train(const QList<DataSample> & trainData, int cyclesToRepeat)
{
    QProgressDialog trainProgress("", "It's not Cancel", 0, 100);
    trainProgress.setValue(0);
    trainProgress.setGeometry(750, 300, 400, 170);
    if(!silentMode)
        trainProgress.show();

    QLabel overallLabel(&trainProgress);
    overallLabel.setGeometry(11, 10, 378, 20);
    overallLabel.setText("Learning cycles progress");
    if(!silentMode)
        overallLabel.show();

    QProgressBar overallBar(&trainProgress);
    overallBar.setGeometry(11, 40, 378, 20);
    overallBar.setValue(0);
    if(!silentMode)
        overallBar.show();

    QLabel cycleLabel(&trainProgress);
    cycleLabel.setGeometry(11, 80, 378, 20);
    cycleLabel.setText("Current cycle progress");
    if(!silentMode)
        cycleLabel.show();

    for(int c = 0; c < cyclesToRepeat; c++)
    {
        for(uint n = 0; n < trainData.size(); n++)
        {
            int choice = Random::Range(trainData.size());
            SetInput(trainData[choice]);
            Step(true, trainData[choice].type);

            if(!silentMode)
            {
                trainProgress.setValue(100 * n / trainData.size());
                QApplication::processEvents();
            }
        }

        if(!silentMode)
        {
            overallBar.setValue(100 * (c + 1) / cyclesToRepeat);
            QApplication::processEvents();
        }
        else
            emit TrainProgress(100 * (c + 1) / cyclesToRepeat);
    }
}
//------------------------------------------------------------------------------
void Net::Step(const bool train, int sampleType)
{
    QList<QPair<double, Sensor *> > sensorLadder;

    //#pragma omp parallel for
    for(int i = 0; i < SENSOR_N; i++)
    {
        sensors[i].UpdateSignal();
        sensors[i].SetState(Sensor::UNDEFINED);
        sensorLadder << QPair<double, Sensor *>(sensors[i].GetSignal(), &sensors[i]);
    }
    //for(int i = 0; i < SENSOR_N; i++)
    //{
    //	sensorLadder << QPair<double, Sensor *>(sensors[i].GetSignal(), &sensors[i]);
    //}

    qSort(sensorLadder);

    const int WINNERS_N = SENSOR_N * netParams.ACTIVE_SENSOR_PERCENT;

    int winners = 0;
    for(uint i = 0; winners < WINNERS_N && i < SENSOR_N; i++)
    {
        if(sensorLadder[SENSOR_N - 1 - i].second->GetState() == Sensor::UNDEFINED)
        {
            sensorLadder[SENSOR_N - 1 - i].second->SetState(Sensor::ACTIVE);
            SupressNeighbours(sensorLadder[SENSOR_N - 1 - i].second);
            winners++;
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
        //#pragma omp parallel for
        for(int i = 0; i < SENSOR_N; i++)
            if(sensorLadder[i].second->GetState() == Sensor::ACTIVE)
            {
                sensorLadder[i].second->UpdateAsWinner(realActiveSensorsPercent);
                sensorLadder[i].second->IncreaseTypeCount(sampleType);
            }
            else
                sensorLadder[i].second->UpdateAsLooser();
    else
    {
        //double resultTable[TYPES_N] = { 0.0 };
        double resultTable[TYPES_N];
        for(int i = 0; i < TYPES_N; i++)
            resultTable[i] = 1.0;

        double sum = 0.0;
        //double product = 1.0;
        for(uint i = 0; i < SENSOR_N; i++)
            if(sensorLadder[i].second->GetState() == Sensor::ACTIVE)
            {
                for(int t = 0; t < TYPES_N; t++)
                    resultTable[t] *= 0.001 + (sensorLadder[i].second->GetTypesTable())[t];
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
    QProgressDialog testProgress("Testing progress", "It's not Cancel", 0, 100);
    testProgress.setValue(0);
    if(!silentMode)
        testProgress.show();

    results.clear();

    for(uint i = 0; i < sensors.size(); i++)
        sensors[i].NormTypesTable();

    for(uint n = 0; n < testData.size(); n++)
    {
        SetInput(testData[n]);
        Step(false, testData[n].type);

        if(!silentMode)
        {
            testProgress.setValue(100 * (n + 1) / testData.size());
            QApplication::processEvents();
        }
        else
            emit TestProgress(100 * (n + 1) / testData.size());
    }

    FindResult();

    if(!silentMode)
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
    if(SACCADE_N != 0)
    {
        QFile resultFile("result.txt");
        resultFile.open(QIODevice::WriteOnly);
        QTextStream out(&resultFile);

        int correctAnswers = 0;

        QMap<int, QList<int> > typeAnswers;

        for(uint i = 0; i < saccadeResults.size(); i++)
        {
            int maxType = -1;
            double maxTypeValue = 0.0;
            for(int t = 0; t < TYPES_N; t++)
            {
                if(saccadeResults[i].result[t] > maxTypeValue)
                {
                    maxTypeValue = saccadeResults[i].result[t];
                    maxType = t;
                }
            }

            if(typeAnswers.find(saccadeResults[i].type) == typeAnswers.end())
                for(uint n = 0; n < TYPES_N; n++)
                    typeAnswers[saccadeResults[i].type] << 0;

            typeAnswers[saccadeResults[i].type][maxType]++;

            if(saccadeResults[i].type == maxType)
                correctAnswers++;

            out << saccadeResults[i].type << " - " << maxType << "(" << QString("%1").arg(saccadeResults[i].result[maxType], 1, 'f', 2, QChar(0)) << ")" << " :";
            for(int t = 0; t < TYPES_N; t++)
                out << " " << t << "(" << QString("%1").arg(saccadeResults[i].result[t], 1, 'f', 2, QChar(0)) << ")";
            out << "\r\n";
        }

        out << "\r\n";

        for(QMap<int, QList<int> >::iterator it = typeAnswers.begin(); it != typeAnswers.end(); ++it)
        {
            int sum = 0;
            int correctAnswers = it.value()[it.key()];
            for(uint n = 0; n < TYPES_N; n++)
                sum += it.value()[n];

            out << "Type " << it.key() << " - " << QString("%1").arg(correctAnswers / (double)sum, 1, 'f', 2, QChar(0)) << " : ";
            for(uint n = 0; n < TYPES_N; n++)
                out << n << "(" << it.value()[n] << ") ";
            out << "\r\n";
        }
        out << "\r\nCorrect overall percent : " << correctAnswers / (double)saccadeResults.size() << "\r\n";

        resultFile.close();
    } //end
    else
    {
        QFile resultFile("result.txt");
        resultFile.open(QIODevice::WriteOnly);
        QTextStream out(&resultFile);

        int correctAnswers = 0;

        QMap<int, QList<int> > typeAnswers;

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

            if(typeAnswers.find(results[i].type) == typeAnswers.end())
                for(uint n = 0; n < TYPES_N; n++)
                    typeAnswers[results[i].type] << 0;

            typeAnswers[results[i].type][maxType]++;

            if(results[i].type == maxType)
                correctAnswers++;

            out << results[i].type << " - " << maxType << "(" << QString("%1").arg(results[i].result[maxType], 1, 'f', 2, QChar(0)) << ")" << " :";
            for(int t = 0; t < TYPES_N; t++)
                out << " " << t << "(" << QString("%1").arg(results[i].result[t], 1, 'f', 2, QChar(0)) << ")";
            out << "\r\n";
        }

        out << "\r\n";

        for(QMap<int, QList<int> >::iterator it = typeAnswers.begin(); it != typeAnswers.end(); ++it)
        {
            int sum = 0;
            int correctAnswers = it.value()[it.key()];
            for(uint n = 0; n < TYPES_N; n++)
                sum += it.value()[n];

            out << "Type " << it.key() << " - " << QString("%1").arg(correctAnswers / (double)sum, 1, 'f', 2, QChar(0)) << " : ";
            for(uint n = 0; n < TYPES_N; n++)
                out << n << "(" << it.value()[n] << ") ";
            out << "\r\n";
        }
        out << "\r\nCorrect overall percent : " << correctAnswers / (double)results.size() << "\r\n";

        resultFile.close();
    }
}
//------------------------------------------------------------------------------
void Net::FindResult()
{
    saccadeResults.clear();

    int correctAnswers = 0;

    if(SACCADE_N != 0)
    {
        for(uint i = 0; i < results.size() / SACCADE_N; i++)
        {
            saccadeResults << Result();
            saccadeResults[i].type = results[i * SACCADE_N].type;

            for(uint t = 0; t < TYPES_N; t++)
                saccadeResults[i].result[t] = 1.0;

            for(uint j = 0; j < SACCADE_N; j++)
            {
                for(int t = 0; t < TYPES_N; t++)
                    saccadeResults[i].result[t] *= results[i * SACCADE_N + j].result[t];
            }

            int maxType = -1;
            double maxTypeValue = 0.0;
            for(int t = 0; t < TYPES_N; t++)
            {
                if(saccadeResults[i].result[t] > maxTypeValue)
                {
                    maxTypeValue = saccadeResults[i].result[t];
                    maxType = t;
                }
            }

            if(saccadeResults[i].type == maxType)
                correctAnswers++;
        }

        result = correctAnswers / (double)saccadeResults.size();
    }
    else
    {
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

            if(results[i].type == maxType)
                correctAnswers++;
        }

        result = correctAnswers / (double)results.size();
    }
}
//------------------------------------------------------------------------------
double Net::GetResult() const
{
    return result;
}
//------------------------------------------------------------------------------
const NetParams & Net::GetParams() const
{
    return netParams;
}
//------------------------------------------------------------------------------
