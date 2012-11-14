#ifndef SYNAPSE_H
#define SYNAPSE_H

#include "NetParams.h"
#include "DataSample.h"
#include <QVector>

class Sensor;

class Synapse
{
public:
    Synapse(Sensor * parent
            , const NetParams * netParams
            , DataSample & field_
            , int x
            , int y);

    void IncreasePermanence();
    void DecreasePermanence();
    void DeadIncreasePermanence();

    int GetSignal() const;
    int GetFieldSignal() const;

    int x, y;
    int activeCoeff;

    double GetP() const;

private:
    double p;
    const Sensor * parent;
    const NetParams * netParams;
    int * field;
};

#endif
