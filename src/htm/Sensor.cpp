#include "Sensor.h"
#include "Options.h"

#include "Random.h"
#include <QImage>
#include <QColor>

#include "math.h"
#include "src/debug/debughelper.h"

Sensor::Sensor(const NetParams * netParams_, DataSample & field) : netParams(netParams_), boost(1.0)
{
    int centerX = Random::Range(9, 23);
    int centerY = Random::Range(9, 23);
    int radius = 9 + Random::Range(10);

    int minX = qMax(0, centerX - radius);
    int maxX = qMin(FIELD_X - 1, centerX + radius);
    int minY = qMax(0, centerY - radius);
    int maxY = qMin(FIELD_Y - 1, centerY + radius);

    for(uint i = minY; i <= maxY; i++)
        for(uint j = minX; j <= maxX; j++)
            synapses << Synapse(this, netParams, field, j, i);

    QVector<QVector<int> > occupiedCells(FIELD_Y, QVector<int>(FIELD_X, 0));

    for(uint i = 0; i < SENSOR_START_ACTIVE_SYNAPSE_N; i++)
    {
        int x, y;
        do
        {
            x = Random::Range(minX, maxX);
            y = Random::Range(minY, maxY);
        }
        while(occupiedCells[y][x]);

        occupiedCells[y][x] = 1;

        //synapses << Synapse(this, netParams, field, x, y);
    }

    for(uint n = 0; n < synapses.size(); n++)
    {
        if(occupiedCells[synapses[n].y][synapses[n].x])
            synapses[n].SetPermanence(netParams->ACTIVE_PERMANENCE);
        else
            synapses[n].SetPermanence(0);
    }

    UpdatePosition();

    for(int i = 0; i < TYPES_N; i++)
        typesTable[i] = 0.0;
}
//------------------------------------------------------------------------------
void Sensor::IncreaseTypeCount(int type)
{
    typesTable[type]++;
}
//------------------------------------------------------------------------------
const double * Sensor::GetTypesTable() const
{
    return typesTable;
}
//------------------------------------------------------------------------------
void Sensor::UpdateSignal()
{
    signal = 0.0;

    for(uint i = 0; i < synapses.size(); i++)
        signal += synapses[i].GetSignal();

    if(signal < netParams->TRIVIAL_PATTERN_THRESHOLD)
        signal = 0.0;

    signal *= boost;
}
//------------------------------------------------------------------------------
void Sensor::NormTypesTable()
{
    double sum = 0.0;
    for(int n = 0; n < TYPES_N; n++)
        sum += typesTable[n];

    for(int n = 0; n < TYPES_N; n++)
        typesTable[n] /= sum;
}
//------------------------------------------------------------------------------
double Sensor::GetSignal() const
{
    return signal;
}
//------------------------------------------------------------------------------
void Sensor::GetPosition(int & x_, int & y_, int & r2_) const
{
    x_ = x;
    y_ = y;
    r2_ = r2;
}
//------------------------------------------------------------------------------
void Sensor::UpdateAsWinner(double currentActiveSensorPercent)
{
    for(uint i = 0; i < synapses.size(); i++)
        if(synapses[i].GetFieldSignal())
            synapses[i].IncreasePermanence();
        else
            synapses[i].DecreasePermanence();

    double decreaseValue = pow(netParams->SENSOR_BOOST, (1.0f - currentActiveSensorPercent) / currentActiveSensorPercent);

    boost /= decreaseValue;

    UpdatePosition();
}
//------------------------------------------------------------------------------
void Sensor::UpdateAsLooser()
{
    if(boost >= netParams->DEAD_SENSOR_BOOST)
        for(uint i = 0; i < synapses.size(); i++)
            synapses[i].DeadIncreasePermanence();

    boost *= netParams->SENSOR_BOOST;

    UpdatePosition();
}
//------------------------------------------------------------------------------
void Sensor::UpdatePosition()
{
    QList<Synapse *> activeSynapses;

    int sumX = 0;
    int sumY = 0;
    int activeSynapsesN = 0;
    for(uint i = 0; i < synapses.size(); i++)
    {
        if(synapses[i].activeCoeff)
        {
            sumX += synapses[i].y;
            sumY += synapses[i].x;
            activeSynapsesN++;
            activeSynapses << &synapses[i];
        }
    }

    if(activeSynapsesN != 0)
    {
        x = sumX / (double)activeSynapsesN + 0.5;
        y = sumY / (double)activeSynapsesN + 0.5;

        int sumR = 0;
        for(uint i = 0; i < activeSynapses.size(); i++)
            sumR += ((*activeSynapses[i]).y - y) * ((*activeSynapses[i]).y - y) +
                    ((*activeSynapses[i]).x - x) * ((*activeSynapses[i]).x - x);

        r2 = sumR / activeSynapsesN + 0.5;
    }
    else
    {
        //hope old x, y and radius are norm
    }
}
//------------------------------------------------------------------------------
void Sensor::SetState(State state_)
{
    state = state_;
}
//------------------------------------------------------------------------------
Sensor::State Sensor::GetState() const
{
    return state;
}
//------------------------------------------------------------------------------
void Sensor::FormImage(QDir dir, int sensorId)
{
    QImage sensorImage(FIELD_X, FIELD_Y, QImage::Format_RGB32);
    sensorImage.fill(Qt::white);

    for(uint i = 0; i < synapses.size(); i++)
    {
        int pColour = (1.0 - synapses[i].GetP()) * 250;
        QColor colour;
        //if(synapses[i].activeCoeff)
            //colour = QColor(255, pColour, 255);
        //else
            //colour = QColor(pColour, 255, 255);
        colour = QColor(pColour, pColour, pColour);
        // swap x and y for Alexander
        sensorImage.setPixel(synapses[i].y, synapses[i].x, colour.rgb());
    }

    sensorImage.save(dir.absoluteFilePath("sensor_" + QString("%1").arg(sensorId, 4, 10, QChar('0')) + ".png"));
}
//------------------------------------------------------------------------------
