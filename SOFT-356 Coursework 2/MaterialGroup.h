#pragma once

#include <vector>

// Structure to store the material name and indexes for specific parts of the object
struct MaterialGroup {
	string materialName;
	vector< pair <unsigned long, unsigned long> > indexPairsArray;
};