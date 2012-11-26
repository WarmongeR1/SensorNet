#include "GenomOperator.h"
#include "Random.h"

NetParamsInitializer minValues;
NetParamsInitializer maxValues;

GenomValuesLimitsInitializer initializer;

GenomValuesLimitsInitializer::GenomValuesLimitsInitializer()
{
	minValues.ActivePermanence = 0.2;
	maxValues.ActivePermanence = 0.8;
	minValues.ActiveSensorPercent = 0.01;
	maxValues.ActiveSensorPercent = 0.5;
	minValues.ActiveSynapseStartPercent = 0.05;
	maxValues.ActiveSynapseStartPercent = 1.0;
	minValues.DeadPermanenceIncrease = 0.001;
	maxValues.DeadPermanenceIncrease = 0.5;
	minValues.DeadSensorBoost = 1.1;
	maxValues.DeadSensorBoost = 10000.0;
	minValues.PermanenceDecrease = 0.0001;
	maxValues.PermanenceDecrease = 0.5;
	minValues.PermanenceIncrease = 0.0001;
	maxValues.PermanenceIncrease = 0.5;
	minValues.SensorBoost = 1.0001;
	maxValues.SensorBoost = 10.0;
	minValues.SensorInhibitionPercent = 0.1;
	maxValues.SensorInhibitionPercent = 1.0;
	minValues.TrivialPatternTreshold = 0;
	maxValues.TrivialPatternTreshold = 100;
}

NetParams GenomOperator::Create()
{
	NetParamsInitializer genom;
	genom.ActivePermanence = Random::Range(minValues.ActivePermanence, maxValues.ActivePermanence);
	genom.ActiveSensorPercent = Random::Range(minValues.ActiveSensorPercent, maxValues.ActiveSensorPercent);

	//TO DO

	return NetParams(genom);
}

NetParams GenomOperator::Mix(const NetParams & params1, const NetParams & params2)
{
//TO DO
	NetParamsInitializer genom;
	return NetParams(genom);
}

NetParams GenomOperator::Mutate(const NetParams & params)
{
//TO DO
	NetParamsInitializer genom;
	return NetParams(genom);
}
