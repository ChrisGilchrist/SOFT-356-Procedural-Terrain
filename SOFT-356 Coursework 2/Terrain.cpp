#pragma once

#include "Terrain.h"

// Private
void Terrain::initVAO()
{
	//Create VAO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//GEN EBO AND BIND AND SEND DATA
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * this->indices.size(), this->indices.data(), GL_STATIC_DRAW);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	//SET VERTEXATTRIBPOINTERS
	//Position	
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		nullptr
	);
	glEnableVertexAttribArray(0);

	/* Texture*/
	glGenBuffers(1, &this->textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->textureCoords.size() * sizeof(float), textureCoords.data(), GL_STATIC_DRAW);
	
	//Texcoords	
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		nullptr
	);
	glEnableVertexAttribArray(1);

	/* Normals*/
	glGenBuffers(1, &this->normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
	
	//Normals	
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		nullptr
	);
	glEnableVertexAttribArray(2);


	//BIND VAO 0
	glBindVertexArray(0);
}

void Terrain::updateUniforms(Shader* shader)
{
	shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
}

void Terrain::updateModelMatrix()
{
	this->ModelMatrix = mat4(1.f);
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
	this->ModelMatrix = glm::rotate(this->ModelMatrix, radians(this->rotation.x), vec3(1.f, 0.f, 0.f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, radians(this->rotation.y), vec3(0.f, 1.f, 0.f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, radians(this->rotation.z), vec3(0.f, 0.f, 1.f));
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin);
	this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
}


// Public 
Terrain::Terrain(
	int gridX,
	int gridZ,
	vec3 position = vec3(0.f),
	vec3 origin = vec3(0.f),
	vec3 rotation = vec3(0.f),
	vec3 scale = vec3(1.f)
) 
{
	this->position = position;
	this->origin = origin;
	this->rotation = rotation;
	this->scale = scale;
	this->x = gridX * SIZE;
	this->z = gridZ * SIZE;

	// This method needs to create the array of Vertex's
	this->generateTerrain(vertices, normals, textureCoords, indices);
	this->initVAO();
	this->updateModelMatrix();
}

Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->vertexBuffer);
	glDeleteBuffers(1, &this->normalBuffer);
	glDeleteBuffers(1, &this->textureBuffer);
	glDeleteBuffers(1, &this->EBO);

	// clear the arrays of information
	//delete[] this->vertexArray;
	//delete[] this->indexArray;

	vertices.clear();
	normals.clear();
	textureCoords.clear();
	indices.clear();
}


void Terrain::generateTerrain(std::vector<float> &vertices, std::vector<float> &normals, vector<float> &textureCoords, vector<int> &indices) {

	// Create terrainGenerator
	HeightsGenerator heightsGenerator = HeightsGenerator();

	// Load in the height map
	//stb::image image{ "heightMap/heightmap.png", 4 };
	
	heights.resize(VERTEX_COUNT);
	for (auto& h : heights) h.resize(VERTEX_COUNT);

	vertices.resize(count * 3);
	normals.resize(count * 3);
	textureCoords.resize(count * 2);
	indices.resize(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

	int vertexPointer = 0;
	for (int i = 0; i < VERTEX_COUNT; i++) {
		for (int j = 0; j < VERTEX_COUNT; j++) {
			vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
			
			float height = getHeight(j, i, heightsGenerator);
			heights[j][i] = height;
			vertices[vertexPointer * 3 + 1] = height;
			vertices[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
			
			vec3 normal = calculateNormal(j, i, heightsGenerator);
			normals[vertexPointer * 3] = normal.x;
			normals[vertexPointer * 3 + 1] = normal.y;
			normals[vertexPointer * 3 + 2] = normal.z;
			
			textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
			textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
			vertexPointer++;
		}
	}
	int pointer = 0;
	for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
		for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
			int topLeft = (gz * VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}
}

float Terrain::getHeight(int x, int z, HeightsGenerator generator) {
	return generator.generateHeight(x, z);
}

vec3 Terrain::calculateNormal(int x, int z, HeightsGenerator generator) {
	float heightL = getHeight(x - 1, z, generator);
	float heightR = getHeight(x + 1, z, generator);
	float heightD = getHeight(x, z - 1, generator);
	float heightU = getHeight(x, z + 1, generator);

	vec3 normal{ heightL - heightR, 2.0f, heightD - heightU };
	normal = normalize(normal);

	return normal;
}

float Terrain::getHeightOfTerrain(float worldX, float worldZ) {
	float terrainX = worldX - x;
	float terrainZ = worldZ - z;

	float gridSquareSize = SIZE / ((float)heights.size() / 1);

	int gridX = (int)floor(terrainX / gridSquareSize);
	int gridZ = (int)floor(terrainZ / gridSquareSize);
	if (gridX >= heights.size() - 1 || gridZ > heights.size() - 1 ||
		gridX < 0 || gridZ < 0) {
		return 0;
	}

	float xCoord = fmod(terrainX, gridSquareSize) / gridSquareSize;
	float zCoord = fmod(terrainZ, gridSquareSize) / gridSquareSize;
	float answer = 0.0f;

	if (xCoord <= (1 - zCoord)) {
		answer = barrycentric(
			glm::vec3{ 0, heights[gridX][gridZ], 0 },
			glm::vec3{ 1, heights[gridX + 1][gridZ], 0 },
			glm::vec3{ 0, heights[gridX][gridZ + 1], 1 },
			glm::vec2{ xCoord, zCoord });
	}
	else {
		answer = barrycentric(
			glm::vec3{ 0, heights[gridX + 1][gridZ], 0 },
			glm::vec3{ 1, heights[gridX + 1][gridZ + 1], 1 },
			glm::vec3{ 0, heights[gridX][gridZ + 1], 1 },
			glm::vec2{ xCoord, zCoord });
	}

	// Maybe add a little offset as for some reason it is slightly off//
	return answer;
}


float Terrain::getX() {
	return this->x;
}

float Terrain::getZ() {
	return this->z;
}



void Terrain::render(Shader* shader)
{
	//Update uniforms
	this->updateModelMatrix();
	this->updateUniforms(shader);

	// Send texture to the shader
	shader->set1i(0, "backgroundTexture");
	shader->set1i(1, "rTexture");
	shader->set1i(2, "gTexture");
	shader->set1i(3, "bTexture");
	shader->set1i(4, "blendMap");

	//Bind VAO
	glBindVertexArray(this->VAO);

	shader->use();

	// Bind terrain and draw
	grassTex.bind(0);
	dirtTex.bind(1);
	grassFlowerTex.bind(2);
	pathTex.bind(3);
	blendMapTex.bind(4);

	// Draw the terrain
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


// Useful Math functions
float Terrain::barrycentric(const glm::vec3& p1, const glm::vec3& p2,
	const glm::vec3& p3, const glm::vec2& pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}