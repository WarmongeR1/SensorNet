#ifndef SENSOR_H
#define SENSOR_H

#include "Synapse.h"
#include "DataSample.h"


#include <QVector>
#include <QList>
#include <QDir>
#include <QImage>


class Sensor
{

public:
    enum State { UNDEFINED = 0, ACTIVE = 1, PASSIVE = 2 };

    Sensor(const NetParams * const netParams
           , DataSample & field);


    void UpdateSignal();
    double GetSignal() const;
    void GetPosition(int & x, int & y, int & r2) const;

    void UpdateAsWinner(double currentActiveSensorPercent);
    void UpdateAsLooser();

    void SetState(State state);
    State GetState() const;

    void FormImage(QDir dir, int sensorId);

    void IncreaseTypeCount(int type);

    const double * GetTypesTable() const;


private:
    void UpdatePosition();

    QList<Synapse> synapses;
    const NetParams * netParams;

    double boost;
    int x, y;
    int r2;

    double signal;
    State state;

    double typesTable[TYPES_N];
};

#endif
