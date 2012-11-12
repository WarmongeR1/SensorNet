#ifndef RANDOM_H
#define RANDOM_H

class Random
{
public:
	static int Range(int range);
	static int Range(int min, int max);
	static double Range(double min, double max);

	static bool Chance(double percent);
};

class RandomInit
{
public:
	RandomInit();
};

#endif
