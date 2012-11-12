#include "Synapse.h"
#include "Random.h"

Synapse::Synapse(Sensor * parent_, const NetParams * const netParams_, DataSample & field_, int x_, int y_) : parent(parent_), netParams(netParams_), field(&(field_.data[x_][y_])), x(x_), y(y_)
{
    if(Random::Chance(netParams->ACTIVE_SYNAPSE_START_PERCENT))
	{
        p = netParams->ACTIVE_PERMANENCE;
		activeCoeff = 1;
	}
	else
	{
		p = 0.0;
		activeCoeff = 0;
	}
}

void Synapse::IncreasePermanence()
{
    p += netParams->PERMANENCE_INCREASE;
	if(p > 1.0)
		p = 1.0;
    if(p >= netParams->ACTIVE_PERMANENCE)
		activeCoeff = 1;
}

void Synapse::DecreasePermanence()
{
    p -= netParams->PERMANENCE_DECREASE;
	if(p < 0.0)
		p = 0.0;
    if(p < netParams->ACTIVE_PERMANENCE)
		activeCoeff = 0;
}

void Synapse::DeadIncreasePermanence()
{
    p += netParams->DEAD_PERMANENCE_INCREASE;
	if(p > 1.0)
		p = 1.0;
    if(p >= netParams->ACTIVE_PERMANENCE)
		activeCoeff = 1;
}

int Synapse::GetSignal() const
{
    return activeCoeff * (*field);
}

int Synapse::GetFieldSignal() const
{
    return *field;
}

double Synapse::GetP() const
{
	return p;
}
