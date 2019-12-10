#pragma once

#include "Mesh.h"

#include "utils.h"

#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "MaterialGroup.h"
#include "stb_image.hpp"

class Terrain
{
private:

	float SIZE = 800;
	int VERTEX_COUNT = 128;
	const float MAX_HEIGHT = 40;
	const float MAX_PIXEL_COLOUR = 256 * 256 * 256;

	int count = 0;
	//static const int count = VERTEX_COUNT * VERTEX_COUNT;
	vector<float> vertices = vector<float>(0);
	vector<float> normals =vector<float>(0);
	vector<float> textureCoords = vector<float>(0);
	vector<int> indices = vector<int>(0);

	std::vector<std::vector<float>> heights;

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

	Texture* texture;

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

	void generateTerrain(std::vector<float>& vertices, std::vector<float>& normals, vector<float>& textureCoords, vector<int>& indices);

	float getHeight(int x, int z, const stb::image& image);

	vec3 calculateNormal(int x, int z, const stb::image& image);

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
