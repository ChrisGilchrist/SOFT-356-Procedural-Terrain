#pragma once

#include "utils.h"

#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "MaterialGroup.h"

class Light
{
private:
	float* vertexArray;
	unsigned nrOfVertices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	vec3 position;
	vec3 origin;
	vec3 rotation;
	vec3 scale;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	mat4 ModelMatrix;

	// Private
	void initVAO();

	void updateUniforms(Shader* shader);

	void updateModelMatrix();

public:
	Light(
		vec3 position,
		vec3 origin,
		vec3 rotation,
		vec3 scale,
		vec3 ambient,
		vec3 diffuse,
		vec3 specular
	);

	Light(const Light& obj);

	~Light();

	// Setters
	void setPosition(const vec3 position);

	void setOrigin(const vec3 origin);

	void setRotation(const vec3 rotation);

	void setScale(const vec3 setScale);

	void setAmbient(const vec3 setAmbient);

	void setDiffuse(const vec3 setDiffuse);

	void setSpecular(const vec3 setSpecular);

	// Getters
	vec3 getPosition();

	vec3 getOrigin();

	vec3 getRotation();

	vec3 getScale();

	vec3 getAmbient();

	vec3 getDiffuse();
	
	vec3 getSpecular();


	// Control the light
	void move(const vec3 position);

	void rotate(const vec3 rotation);

	void scaleUp(const vec3 scale);

	//  Render light to the scene
	void render(Shader* shader);
};