#ifndef GENOM_OPERATOR_H
#define GENOM_OPERATOR_H

#include "NetParams.h"

class GenomValuesLimitsInitializer
{
public:
	GenomValuesLimitsInitializer();
};

class GenomOperator
{
public:
	static NetParams Create();
	static NetParams Mix(const NetParams & params1, const NetParams & params2);
	static NetParams Mutate(const NetParams & params);
private:

};

#endif
