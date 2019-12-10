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
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(float), textureCoords.data(), GL_STATIC_DRAW);
	
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
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
	
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
	int gridY,
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
	this->y = gridY * SIZE;

	// This method needs to create the array of Vertex's
	this->generateTerrain(vertices, normals, textureCoords, this->indices);
	this->initVAO();
	this->updateModelMatrix();
}

Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->vertexBuffer);
	//glDeleteBuffers(1, &this->normalBuffer);
	//glDeleteBuffers(1, &this->textureBuffer);

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

	// Create texture for terrain
	texture = &Texture("grass.png", GL_TEXTURE_2D);

	int h, w;

	// Load in the height map
	stb::image image{ "heightMap/heightmap.png", 4 };
	int VERTEX_COUNT = image.height();
	heights.resize(VERTEX_COUNT);
	for (auto& h : heights) h.resize(VERTEX_COUNT);

	count = VERTEX_COUNT * VERTEX_COUNT;
	vertices.resize(count * 3);
	normals.resize(count * 3);
	textureCoords.resize(count * 2);
	indices.resize(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

	int vertexPointer = 0;
	for (int i = 0; i < VERTEX_COUNT; i++) {
		for (int j = 0; j < VERTEX_COUNT; j++) {
			vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
			
			float height = getHeight(j, i, image);
			heights[j][i] = height;
			vertices[vertexPointer * 3 + 1] = height;
			//vertices[vertexPointer * 3 + 1] = 0;

			vertices[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
			
			vec3 normal = calculateNormal(j, i, image);
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

float Terrain::getHeight(int x, int z, const stb::image& image) {
	if (x < 0 || x > image.height() || z < 0 || z > image.height()) {
		return 0;
	}

	uint32_t val = image.get_rgb(x, z);

	double height = val;
	height /= (MAX_PIXEL_COLOUR / 2);
	height -= 1.0;
	height *= MAX_HEIGHT;
	//std::cout << height << std::endl;

	return height;
}

vec3 Terrain::calculateNormal(int x, int z, const stb::image& image) {
	float height_l = getHeight(x - 1, z, image);
	float height_r = getHeight(x + 1, z, image);
	float height_d = getHeight(x, z - 1, image);
	float height_u = getHeight(x, z + 1, image);

	glm::vec3 normal{ height_l - height_r, 2.0f, height_d - height_u };
	normal = glm::normalize(normal);

	return normal;
}

void Terrain::setPosition(const vec3 position)
{
	this->position = position;
}

void Terrain::setOrigin(const vec3 origin)
{
	this->origin = origin;
}

void Terrain::setRotation(const vec3 rotation)
{
	this->rotation = rotation;
}

void Terrain::setScale(const vec3 setScale)
{
	this->scale = scale;
}


void Terrain::move(const vec3 position)
{
	this->position += position;
}

void Terrain::rotate(const vec3 rotation)
{
	this->rotation += rotation;
}

void Terrain::scaleUp(const vec3 scale)
{
	this->scale += scale;
}



float Terrain::getX() {
	return this->x;
}

float Terrain::getY() {
	return this->y;
}

void Terrain::render(Shader* shader)
{
	//Update uniforms
	this->updateModelMatrix();
	this->updateUniforms(shader);

	// Send texture to the shader
	shader->set1i(0, "terrainTexture");

	shader->use();

	//Bind VAO
	glBindVertexArray(this->VAO);

	// Bind terrain and draw
	texture->bind(0);

	// Draw the terrain
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
