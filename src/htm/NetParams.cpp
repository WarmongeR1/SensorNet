#include "NetParams.h"

NetParams::NetParams(NetParamsInitializer & from) :
    ACTIVE_SYNAPSE_START_PERCENT(from.ActiveSynapseStartPercent),
    ACTIVE_PERMANENCE(from.ActivePermanence),
    PERMANENCE_INCREASE(from.PermanenceIncrease),
    PERMANENCE_DECREASE(from.PermanenceDecrease),
    DEAD_PERMANENCE_INCREASE(from.DeadPermanenceIncrease),

    TRIVIAL_PATTERN_THRESHOLD(from.TrivialPatternTreshold),
    ACTIVE_SENSOR_PERCENT(from.ActiveSensorPercent),
    SENSOR_INHIBITION_PERCENT(from.SensorInhibitionPercent),
    SENSOR_BOOST(from.SensorBoost),
    DEAD_SENSOR_BOOST(from.DeadSensorBoost)
{
}
