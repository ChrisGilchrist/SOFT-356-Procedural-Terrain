#pragma once

#include "Mesh.h"
#pragma once

#include "utils.h"

#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "MaterialGroup.h"

class Terrain
{
private:
	float SIZE = 800;
	static const int VERTEX_COUNT = 128;

	static const int count = VERTEX_COUNT * VERTEX_COUNT;

	vector<float> vertices = vector<GLfloat>(count * 3);
	vector<float> normals =vector<GLfloat>(count * 3);
	vector<float> textureCoords = vector<GLfloat>(count * 2);
	vector<int> indices = vector<int>(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

	//int nrOfVertices;
	unsigned int nrOfIndices;

	float x;
	float y;

	Vertex* vertexArray;
	unsigned int nrOfVertices;
	GLuint* indexArray;
	//unsigned nrOfIndices;

	GLuint VAO;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint textureBuffer;
	GLuint EBO;

	vec3 position;
	vec3 origin;
	vec3 rotation;
	vec3 scale;

	mat4 ModelMatrix;

	std::vector<GLuint> m_vaos;
	std::vector<GLuint> m_vbos;

	void initVAO();

	void updateUniforms(Shader* shader);

	void updateModelMatrix();

public:
	Terrain(
		int gridX,
		int gridY,
		vec3 position,
		vec3 origin,
		vec3 rotation,
		vec3 scale
		//Texture texture
	);

	~Terrain();

	void generateTerrain(std::vector<GLfloat>& vertices, std::vector<GLfloat>& normals, vector<GLfloat>& textureCoords, vector<int>& indices);

	// Setters
	void setPosition(const vec3 position);

	void setOrigin(const vec3 origin);

	void setRotation(const vec3 rotation);

	void setScale(const vec3 setScale);


	// Control the mesh
	void move(const vec3 position);

	void rotate(const vec3 rotation);

	void scaleUp(const vec3 scale);

	// Getters
	float getX();

	float getY();

	//Texture getTexture();


	// Render the mesh to the scene
	void render(Shader* shader);

};
