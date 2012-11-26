#ifndef NET_PARAMS_H
#define NET_PARAMS_H

struct NetParamsInitializer
{
    double ActiveSynapseStartPercent;
    double ActivePermanence;
    double PermanenceIncrease;
    double PermanenceDecrease;
    double DeadPermanenceIncrease;

    double TrivialPatternTreshold;
    double ActiveSensorPercent;
    double SensorInhibitionPercent;
    double SensorBoost;
    double DeadSensorBoost;
};

struct NetParams
{
    NetParams(NetParamsInitializer & initializer);

    const double ACTIVE_SYNAPSE_START_PERCENT;
    const double ACTIVE_PERMANENCE;
    const double PERMANENCE_INCREASE;
    const double PERMANENCE_DECREASE;
    const double DEAD_PERMANENCE_INCREASE;

    const double TRIVIAL_PATTERN_THRESHOLD;
    const double ACTIVE_SENSOR_PERCENT;
    const double SENSOR_INHIBITION_PERCENT;
    const double SENSOR_BOOST;
    const double DEAD_SENSOR_BOOST;
};

#endif
