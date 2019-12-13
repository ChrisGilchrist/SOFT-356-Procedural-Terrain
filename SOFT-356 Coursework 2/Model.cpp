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
	//this->mesh->move(this->position);
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

void Model::move(string direction, float delta)
{
	this->mesh->move(direction, delta);
}


float Model::getPositionX() {
	return this->mesh->getPosition().x;
}

float Model::getPositionY() {
	return this->mesh->getPosition().y;
}

float Model::getPositionZ() {
	return this->mesh->getPosition().z;
}

void Model::setPositionX(float postionX) {
	this->mesh->setPositionX(postionX);
}

void Model::setPositionY(float postionY) {
	// add a little offset to the y so it is higher off groud
	// postionY = postionY + 2;
	this->mesh->setPositionY(postionY);
}

void Model::setPositionZ(float postionZ) {
	this->mesh->setPositionZ(postionZ);
}


float Model::getRotationY() {
	return this->mesh->getRotation().y;
}

void Model::render(Shader* shader, vector<Texture*> textures)
{
	this->mesh->render(shader, materials, textures);
}