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
	const float AMPLITUDE = 300.0f;

	unsigned int seed;

	int OCTAVES = 5;
	float ROUGHNESS = 0.35f;
	int xOffset = 0;
	int zOffset = 0;

public:

	HeightsGenerator();

	float generateHeight(int x, int z);

	float getSmoothNoise(int x, int z);

	float getInterpolatedNoise(float x, float y);

	float interpolate(float a, float b, float blend);

	float getNoise(int x, int z);


	// Util
	float genRandomSeed(int max);

	float genRandomNumber();



};

