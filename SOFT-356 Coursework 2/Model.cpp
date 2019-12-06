#include "Model.h"

// Public
Model::Model(
	vec3 position,
	vector<Material*> materials,
	Mesh* mesh
)
{
	this->position = position;
	for (auto i : materials)
	{
		this->materials.push_back(new Material(*i));
	}

	this->mesh = new Mesh(*mesh);
	this->mesh->move(this->position);
	this->mesh->setOrigin(this->position);
}

Model::~Model()
{
	delete mesh;
}


void Model::rotate(const vec3 rotation)
{
	this->mesh->rotate(rotation);
}

void Model::scale(const vec3 scale)
{
	this->mesh->scaleUp(scale);
}



void Model::render(Shader* shader, vector<Texture*> textures)
{
	this->mesh->render(shader, materials, textures);
}