#pragma once

#include "Mesh.h"

#include "utils.h"

#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "MaterialGroup.h"
#include "stb_image.hpp"
#include "HeightsGenerator.h"

class Terrain
{
private:

	float SIZE = 1600;
	float VERTEX_COUNT = 128;
	const float MAX_PIXEL_COLOUR = 256 * 256 * 256;

	int count = VERTEX_COUNT * VERTEX_COUNT;
	vector<float> vertices = vector<float>(0);
	vector<float> normals =vector<float>(0);
	vector<float> textureCoords = vector<float>(0);
	vector<int> indices = vector<int>(0);

	std::vector<std::vector<float>> heights;

	unsigned int nrOfIndices;

	float x;
	float z;

	Vertex* vertexArray;
	unsigned int nrOfVertices;
	GLuint* indexArray;

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

	Texture grassTex = Texture("terrain/grass.png", GL_TEXTURE_2D);
	Texture dirtTex = Texture("terrain/mud.png", GL_TEXTURE_2D);
	Texture grassFlowerTex = Texture("terrain/grassFlowers.png", GL_TEXTURE_2D);
	Texture pathTex = Texture("terrain/path.png", GL_TEXTURE_2D);
	Texture blendMapTex = Texture("terrain/blendMap.png", GL_TEXTURE_2D);

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
	);

	~Terrain();

	void generateTerrain(std::vector<float>& vertices, std::vector<float>& normals, vector<float>& textureCoords, vector<int>& indices);

	float getHeight(int x, int z, HeightsGenerator heightsGenerator);

	float getHeightOfTerrain(float worldX, float worldY);

	vec3 calculateNormal(int x, int z, HeightsGenerator heightsGenerator);

	// Getters
	float getX();

	float getZ();

	// Render the terrain to the scene
	void render(Shader* shader);

	// Math functions
	float barrycentric(const glm::vec3& p1, const glm::vec3& p2,
		const glm::vec3& p3, const glm::vec2& pos);

};
