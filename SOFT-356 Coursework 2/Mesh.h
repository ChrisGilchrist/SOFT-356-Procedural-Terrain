#pragma once

#include "utils.h"

#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "MaterialGroup.h"

class Mesh
{
private:
	Vertex* vertexArray;
	unsigned nrOfVertices;
	GLuint* indexArray;
	unsigned nrOfIndices;

	MaterialGroup* materialGroups;
	unsigned nrOfMatGroups;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	vec3 position;
	vec3 origin;
	vec3 rotation;
	vec3 scale;

	float RUN_SPEED = 20;
	float TURN_SPEED = 160;
	float GRAVITY = -50;
	float JUMP_POWER = 30;
	float TERRAIN_HEIGHT = 0;

	float currentSpeed = 0;
	float currentTurnSpeed = 0;
	float upwardsSpeed = 0;


	mat4 ModelMatrix;

	void initVAO();

	void updateUniforms(Shader* shader);

	void updateModelMatrix();

public:
	Mesh();

	Mesh(
		Vertex* vertexArray,
		const unsigned& nrOfVertices,
		GLuint* indexArray,
		const unsigned& nrOfIndices,
		MaterialGroup* materialGroups,
		const unsigned& nrOfMatGroups,
		vec3 position,
		vec3 origin,
		vec3 rotation,
		vec3 scale
	);

	Mesh(const Mesh& obj);

	~Mesh();

	
	// Setters
	void setPosition(const vec3 position);

	void setOrigin(const vec3 origin);

	void setRotation(const vec3 rotation);

	void setScale(const vec3 setScale);


	// Control the mesh
	void move(string direction, float delta);

	void checkDirection(string direction);

	void rotate(const vec3 rotation);

	void scaleUp(const vec3 scale);

	vec3 getPosition();

	void setPositionX(float postionX);

	void setPositionY(float postionY);

	void setPositionZ(float postionZ);


	void increaseRotation(float dx, float dy, float dz);

	void increasePosition(float dx, float dy, float dz);


	vec3 getRotation();


	// Render the mesh to the scene
	void render(Shader* shader, vector<Material*> materials, vector<Texture*> textures);

};