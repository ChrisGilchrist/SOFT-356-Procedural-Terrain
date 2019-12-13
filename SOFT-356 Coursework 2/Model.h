#pragma once

#include "utils.h"

#include"Mesh.h"
#include"Texture.h"
#include"Shader.h"
#include"Material.h"

class Model
{
private:
	vector<Material*> materials;
	Mesh* mesh;
	vec3 position;

public:
	Model(
		vec3 position,
		vector<Material*> materials,
		Mesh* mesh
	);

	~Model();


	// Control the model
	void rotate(const vec3 rotation);

	void scale(const vec3 scale);

	void move(const vec3 position);


	float getPositionX();

	float getPositionY();

	float getPositionZ();

	void setPositionX(float postionX);

	void setPositionY(float postionY);

	void setPositionZ(float postionZ);


	float getRotationY();


	// Render the model to the scene
	void render(Shader* shader, vector<Texture*> textures);
};