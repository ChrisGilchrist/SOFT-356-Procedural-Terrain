#include "HeightsGenerator.h"

// Private

// Public
HeightsGenerator::HeightsGenerator()
{

	xOffset = 0 * (128 - 1);
	zOffset = 0 * (128 - 1);

	// Generate the seed number
	srand(time(NULL));
	float randNum = genRandomSeed(MAX_VALUE);
	// Set the seed
	this->seed = randNum;
}

float HeightsGenerator::generateHeight(int x, int z) {

	float total = 0;
	float d = (float)pow(2, OCTAVES - 1);
	for (int i = 0; i < OCTAVES; i++) {
		float freq = (float)(pow(2, i) / d);
		float amp = (float)pow(ROUGHNESS, i) * AMPLITUDE;
		total += getInterpolatedNoise((x + xOffset) * freq, (z + zOffset) * freq) * amp;
	}
	return total;

}

float HeightsGenerator::getInterpolatedNoise(float x, float z) {
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
	float f = (float)(1.f - cos(theta)) * 0.5f;
	return a * (1.f - f) + b * f;
}

float HeightsGenerator::getSmoothNoise(int x, int z) {

	float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1)
		+ getNoise(x + 1, z + 1)) / 16.f;

	float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1)
		+ getNoise(x, z + 1)) / 8.f;

	float center = getNoise(x, z) / 4.f;

	return corners + sides + center;

}

float HeightsGenerator::getNoise(int x, int z) {
	srand(x * 49632 + z * 325176 + seed);
	float randNum = genRandomNumber();
	return randNum * 2.0f - 1.0f;
}

// Generate seed between 0 and 100000000
float HeightsGenerator::genRandomSeed(int max) {
	int f = std::rand() % max + 1;
	return f;
}

// Generate number between 0 and 1 for terrain height
float HeightsGenerator::genRandomNumber() {
	//srand(time(NULL));	
	//float f = rand() % max;
	
	//float x = rand();
	//float f = (float)x / 1;
	float f = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return f;
}