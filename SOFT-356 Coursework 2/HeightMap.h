#pragma once

#include "utils.h"
class Heightmap
{
public:
	struct HillAlgorithmParameters
	{
		HillAlgorithmParameters(int rows, int columns, int numHills, int hillRadiusMin, int hillRadiusMax, float hillMinHeight, float hillMaxHeight)
		{
			this->rows = rows;
			this->columns = columns;
			this->numHills = numHills;
			this->hillRadiusMin = hillRadiusMin;
			this->hillRadiusMax = hillRadiusMax;
			this->hillMinHeight = hillMinHeight;
			this->hillMaxHeight = hillMaxHeight;
		}

		int rows;
		int columns;
		int numHills;
		int hillRadiusMin;
		int hillRadiusMax;
		float hillMinHeight;
		float hillMaxHeight;
	};

	Heightmap(const HillAlgorithmParameters& params, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

	void createFromHeightData(const std::vector<std::vector<float>>& heightData);

	void render() const override;
	void renderPoints() const override;

	int getRows() const;
	int getColumns() const;
	float getHeight(const int row, const int column) const;

	static std::vector<std::vector<float>> generateRandomHeightData(const HillAlgorithmParameters& params);

private:
	void setUpVertices();
	void setUpTextureCoordinates();
	void setUpNormals();
	void setUpIndexBuffer();

	std::vector<std::vector<float>> _heightData;
	std::vector<std::vector<glm::vec3>> _vertices;
	std::vector<std::vector<glm::vec2>> _textureCoordinates;
	std::vector<std::vector<glm::vec3>> _normals;
	int _rows = 0;
	int _columns = 0;
};