#include "Random.h"

#include <stdlib.h>
#include <time.h>

RandomInit randomInit;

RandomInit::RandomInit()
{
    srand(unsigned(time(0)));
}

int Random::Range(int range)
{
    return rand() % range;
}

int Random::Range(int min, int max)
{
    return min + rand() % (max - min + 1);
}

double Random::Range(double min, double max)
{
    return min + (rand() % 30000) * (max - min) / 30000.0;
}

bool Random::Chance(double percent)
{
    return Range(0.0, 1.0) < percent;
}
