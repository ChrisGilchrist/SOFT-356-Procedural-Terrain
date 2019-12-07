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
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(GLfloat), &vertices, GL_STATIC_DRAW);

	/* Texture
	glGenBuffers(1, &this->textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), &textureCoords, GL_STATIC_DRAW);
	*/

	/* Normals
	glGenBuffers(1, &this->normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals, GL_STATIC_DRAW);
	*/

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
	delete[] this->vertexArray;
	delete[] this->indexArray;

	vertices.clear();
	//normals.clear();
	//textureCoords.clear();
	indices.clear();
}


void Terrain::generateTerrain(std::vector<GLfloat> &vertices, std::vector<GLfloat> &normals, vector<GLfloat> &textureCoords, vector<int> &indices) {

	int vertexPointer = 0;
	for (int i = 0; i < VERTEX_COUNT; i++) {
		for (int j = 0; j < VERTEX_COUNT; j++) {
			vertices[vertexPointer * 3] = (GLfloat)j / ((GLfloat)VERTEX_COUNT - 1) * SIZE;
			vertices[vertexPointer * 3 + 1] = 0;
			vertices[vertexPointer * 3 + 2] = (GLfloat)i / ((GLfloat)VERTEX_COUNT - 1) * SIZE;
			normals[vertexPointer * 3] = 0;
			normals[vertexPointer * 3 + 1] = 1;
			normals[vertexPointer * 3 + 2] = 0;
			textureCoords[vertexPointer * 2] = (GLfloat)j / ((GLfloat)VERTEX_COUNT - 1);
			textureCoords[vertexPointer * 2 + 1] = (GLfloat)i / ((GLfloat)VERTEX_COUNT - 1);
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

	shader->use();

	//Bind VAO
	glBindVertexArray(this->VAO);

	// Draw the terrain
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
	//glActiveTexture(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}
