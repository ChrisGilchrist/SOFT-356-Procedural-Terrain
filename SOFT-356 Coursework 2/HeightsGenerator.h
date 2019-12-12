#pragma once
#define _USE_MATH_DEFINES

#include "utils.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <random>

class HeightsGenerator
{
private:

	const int MAX_VALUE = 10000;
	const float AMPLITUDE = 70.0f;

	unsigned int seed;

public:

	HeightsGenerator();

	float generateHeight(int x, int z);

	float getSmoothNoise(int x, int z);

	float getInterpolateNoise(float x, float y);

	float interpolate(float a, float b, float blend);

	float getNoise(int x, int z);


	// Util
	float genRandomSeed(int max);

	float genRandomNumber();



};

