#include "HeightMap.h"

#include <std>

std::vector<std::vector<float>> Heightmap::generateRandomHeightData(const HillAlgorithmParameters& params)
{
	std::vector<std::vector<float>> heightData(params.rows, std::vector<float>(params.columns, 0.0f));

	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> hillRadiusDistribution(params.hillRadiusMin, params.hillRadiusMax);
	std::uniform_real_distribution<float> hillHeightDistribution(params.hillMinHeight, params.hillMaxHeight);
	std::uniform_int_distribution<int> hillCenterRowIntDistribution(0, params.rows - 1);
	std::uniform_int_distribution<int> hillCenterColIntDistribution(0, params.columns - 1);

	for (int i = 0; i < params.numHills; i++)
	{
		const auto hillCenterRow = hillCenterRowIntDistribution(generator);
		const auto hillCenterCol = hillCenterColIntDistribution(generator);
		const auto hillRadius = hillRadiusDistribution(generator);
		const auto hillHeight = hillHeightDistribution(generator);

		for (auto r = hillCenterRow - hillRadius; r < hillCenterRow + hillRadius; r++)
		{
			for (auto c = hillCenterCol - hillRadius; c < hillCenterCol + hillRadius; c++)
			{
				if (r < 0 || r >= params.rows || c < 0 || c >= params.columns) {
					continue;
				}
				const auto r2 = hillRadius * hillRadius; // r*r ter
				const auto x2x1 = hillCenterCol - c; // (x2-x1) ter
				const auto y2y1 = hillCenterRow - r; // (y2-y1) ter
				const auto height = float(r2 - x2x1 * x2x1 - y2y1 * y2y1);
				if (height < 0.0f) {
					continue;
				}
				const auto factor = height / r2;
				heightData[r][c] += hillHeight * factor;
				if (heightData[r][c] > 1.0f) {
					heightData[r][c] = 1.0f;
				}
			}
		}
	}
	return heightData;
}


{
	if (_isInitialized) {
		deleteMesh();
	}

	_heightData = heightData;
	_rows = _heightData.size();
	_columns = _heightData[0].size();
	_numVertices = _rows * _columns;

	// First, prepare VAO and VBO for vertex dat
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	_vbo.createVBO(_numVertices * getVertexByteSize()); // Preallocate memor
	_vbo.bindVBO();

	if (hasPositions()) {
		setUpVertices();
	}

	if (hasTextureCoordinates()) {
		setUpTextureCoordinates();
	}

	if (hasNormals())
	{
		if (!hasPositions()) {
			setUpVertices();
		}

		setUpNormals();
	}

	setUpIndexBuffer();

	// Clear the data, we won't need it anymor
	_vertices.clear();
	_textureCoordinates.clear();
	_normals.clear();

	// If get here, we have succeeded with generating heightma
	_isInitialized = true;
}


void Heightmap::setUpVertices()
{
	_vertices = std::vector<std::vector<glm::vec3>>(_rows, std::vector<glm::vec3>(_columns));

	for (auto i = 0; i < _rows; i++)
	{
		for (auto j = 0; j < _columns; j++)
		{
			const auto factorRow = float(i) / float(_rows - 1);
			const auto factorColumn = float(j) / float(_columns - 1);
			const auto& vertexHeight = _heightData[i][j];
			_vertices[i][j] = glm::vec3(-0.5f + factorColumn, vertexHeight, -0.5f + factorRow);
		}
		_vbo.addData(_vertices[i].data(), _columns * sizeof(glm::vec3));
	}
}


void Heightmap::setUpTextureCoordinates()
{
	_textureCoordinates = std::vector<std::vector<glm::vec2>>(_rows, std::vector<glm::vec2>(_columns));

	const auto textureStepU = 0.1f;
	const auto textureStepV = 0.1f;

	for (auto i = 0; i < _rows; i++)
	{
		for (auto j = 0; j < _columns; j++) {
			_textureCoordinates[i][j] = glm::vec2(textureStepU * j, textureStepV * i);
		}
		_vbo.addData(_textureCoordinates[i].data(), _columns * sizeof(glm::vec2));
	}
}


void Heightmap::setUpNormals()
{
	_normals = std::vector<std::vector<glm::vec3>>(_rows, std::vector<glm::vec3>(_columns));
	std::vector< std::vector<glm::vec3> > tempNormals[2];
	for (auto i = 0; i < 2; i++) {
		tempNormals[i] = std::vector<std::vector<glm::vec3>>(_rows - 1, std::vector<glm::vec3>(_columns - 1));
	}

	for (auto i = 0; i < _rows - 1; i++)
	{
		for (auto j = 0; j < _columns - 1; j++)
		{
			const auto& vertexA = _vertices[i][j];
			const auto& vertexB = _vertices[i][j + 1];
			const auto& vertexC = _vertices[i + 1][j + 1];
			const auto& vertexD = _vertices[i + 1][j];

			const auto triangleNormalA = glm::cross(vertexB - vertexA, vertexA - vertexD);
			const auto triangleNormalB = glm::cross(vertexD - vertexC, vertexC - vertexB);

			tempNormals[0][i][j] = glm::normalize(triangleNormalA);
			tempNormals[1][i][j] = glm::normalize(triangleNormalB);
		}
	}

	for (auto i = 0; i < _rows; i++)
	{
		for (auto j = 0; j < _columns; j++)
		{
			const auto isFirstRow = i == 0;
			const auto isFirstColumn = j == 0;
			const auto isLastRow = i == _rows - 1;
			const auto isLastColumn = j == _columns - 1;

			auto finalVertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);

			// Look for triangle to the upper-lef
			if (!isFirstRow && !isFirstColumn) {
				finalVertexNormal += tempNormals[0][i - 1][j - 1];
			}

			// Look for triangles to the upper-righ
			if (!isFirstRow && !isLastColumn) {
				for (auto k = 0; k < 2; k++) {
					finalVertexNormal += tempNormals[k][i - 1][j];
				}
			}

			// Look for triangle to the bottom-righ
			if (!isLastRow && !isLastColumn) {
				finalVertexNormal += tempNormals[0][i][j];
			}

			// Look for triangles to the bottom-righ
			if (!isLastRow && !isFirstColumn) {
				for (auto k = 0; k < 2; k++) {
					finalVertexNormal += tempNormals[k][i][j - 1];
				}
			}

			// Store final normal of j-th vertex in i-th ro
			_normals[i][j] = glm::normalize(finalVertexNormal);
		}
		_vbo.addData(_normals[i].data(), _columns * sizeof(glm::vec3));
	}
}

void Heightmap::setUpIndexBufer()
{
	// Create a VBO with heightmap indices
	_indicesVBO.createVBO();
	_indicesVBO.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
	_primitiveRestartIndex = _numVertices;

	for (auto i = 0; i < _rows - 1; i++)
	{
		for (auto j = 0; j < _columns; j++)
		{
			for (auto k = 0; k < 2; k++)
			{
				const auto row = i + k;
				const auto index = row * _columns + j;
				_indicesVBO.addData(&index, sizeof(int));
			}
		}
		// Restart triangle strips
		_indicesVBO.addData(&_primitiveRestartIndex, sizeof(int));
	}

	// Send indices to GPU
	_indicesVBO.uploadDataToGPU(GL_STATIC_DRAW);

	// Calculate total count of indices
	_numIndices = (_rows - 1) * _columns * 2 + _rows - 1;
}


void Heightmap::render() const
{
	if (!_isInitialized) {
		return;
	}

	glBindVertexArray(_vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(_primitiveRestartIndex);

	glDrawElements(GL_TRIANGLE_STRIP, _numIndices, GL_UNSIGNED_INT, 0);
	glDisable(GL_PRIMITIVE_RESTART);
}