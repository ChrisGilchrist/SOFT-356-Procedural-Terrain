#include "HeightsGenerator.h"

// Private

// Public
HeightsGenerator::HeightsGenerator()
{

	// Generate the seed number
	//srand(time(NULL));
	float randNum = genRandomNumber(MAX_VALUE);
	// Set the seed
	this->seed = randNum;

	cout << genRandomNumber(MAX_VALUE);
	cout << genRandomNumber(MAX_VALUE);
	cout << genRandomNumber(MAX_VALUE);

}

float HeightsGenerator::generateHeight(int x, int z) {

	float total = getInterpolateNoise(x / 4, z / 4) * AMPLITUDE;
	total += getInterpolateNoise(x / 2, z / 2) * AMPLITUDE / 3;
	total += getInterpolateNoise(x, z) * AMPLITUDE / 9;
	return total;

}

float HeightsGenerator::getInterpolateNoise(float x, float z) {
	int intX = (int)x;
	int intZ = (int)z;
	float fracX = x - intX;
	float fracZ = z - intZ;

	float v1 = getSmoothNoise(intX, intZ);
	float v2 = getSmoothNoise(intX + 1, intZ);
	float v3 = getSmoothNoise(intX, intZ + 1);
	float v4 = getSmoothNoise(intX + 1, intZ + 1);
	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);
	return interpolate(i1, i2, fracZ);
}

float HeightsGenerator::interpolate(float a, float b, float blend) {
	double theta = blend * M_PI;
	float f = (1 - cos(theta)) * 0.5;
	return a * (1 - f) + b * f;
}

float HeightsGenerator::getSmoothNoise(int x, int z) {
	float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1)
		+ getNoise(x + 1, z + 1)) / 16;

	float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1)
		+ getNoise(x, z + 1)) / 8;

	float center = getNoise(x, z) / 4;

	return corners + sides + center;

}

float HeightsGenerator::getNoise(int x, int z) {
	srand(x * 49632 + z * 325176 + seed);
	return (genRandomNumber(MAX_VALUE)) * 2 - 1;
}


float HeightsGenerator::genRandomNumber(int max) {
	//srand(time(NULL));	
	//float f = rand() % max;
	
	float x = rand();
	float f = (float)x / max;
	return f;
}