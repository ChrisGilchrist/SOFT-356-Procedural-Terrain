#include "Light.h"

// Private
void Light::initVAO()
{
	float vertices[] = {
	 -0.5f, -0.5f, -0.5f,
	  0.5f, -0.5f, -0.5f,
	  0.5f,  0.5f, -0.5f,
	  0.5f,  0.5f, -0.5f,
	 -0.5f,  0.5f, -0.5f,
	 -0.5f, -0.5f, -0.5f,

	 -0.5f, -0.5f,  0.5f,
	  0.5f, -0.5f,  0.5f,
	  0.5f,  0.5f,  0.5f,
	  0.5f,  0.5f,  0.5f,
	 -0.5f,  0.5f,  0.5f,
	 -0.5f, -0.5f,  0.5f,

	 -0.5f,  0.5f,  0.5f,
	 -0.5f,  0.5f, -0.5f,
	 -0.5f, -0.5f, -0.5f,
	 -0.5f, -0.5f, -0.5f,
	 -0.5f, -0.5f,  0.5f,
	 -0.5f,  0.5f,  0.5f,

	  0.5f,  0.5f,  0.5f,
	  0.5f,  0.5f, -0.5f,
	  0.5f, -0.5f, -0.5f,
	  0.5f, -0.5f, -0.5f,
	  0.5f, -0.5f,  0.5f,
	  0.5f,  0.5f,  0.5f,

	 -0.5f, -0.5f, -0.5f,
	  0.5f, -0.5f, -0.5f,
	  0.5f, -0.5f,  0.5f,
	  0.5f, -0.5f,  0.5f,
	 -0.5f, -0.5f,  0.5f,
	 -0.5f, -0.5f, -0.5f,

	 -0.5f,  0.5f, -0.5f,
	  0.5f,  0.5f, -0.5f,
	  0.5f,  0.5f,  0.5f,
	  0.5f,  0.5f,  0.5f,
	 -0.5f,  0.5f,  0.5f,
	 -0.5f,  0.5f, -0.5f,
	};

	//Create VAO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//BIND VAO 0
	glBindVertexArray(0);
}

void Light::updateUniforms(Shader* shader)
{
	shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
}

void Light::updateModelMatrix()
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
Light::Light(
	vec3 position,
	vec3 origin,
	vec3 rotation,
	vec3 scale,
	vec3 ambient,
	vec3 diffuse,
	vec3 specular)
{
	this->position = position;
	this->origin = origin;
	this->rotation = rotation;
	this->scale = scale;

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->initVAO();
	this->updateModelMatrix();
}

Light::Light(const Light& obj)
{
	this->position = obj.position;
	this->origin = obj.origin;
	this->rotation = obj.rotation;
	this->scale = obj.scale;

	this->ambient = obj.ambient;
	this->diffuse = obj.diffuse;
	this->specular = obj.specular;

	this->initVAO();
	this->updateModelMatrix();
}

Light::~Light()
{

}

// Setters
void Light::setPosition(const vec3 position)
{
	this->position = position;
}

void Light::setOrigin(const vec3 origin)
{
	this->origin = origin;
}

void Light::setRotation(const vec3 rotation)
{
	this->rotation = rotation;
}

void Light::setScale(const vec3 setScale)
{
	this->scale = scale;
}

void Light::setAmbient(const vec3 ambient)
{
	this->ambient = ambient;
}

void Light::setDiffuse(const vec3 diffuse)
{
	this->diffuse = diffuse;
}

void Light::setSpecular(const vec3 specular)
{
	this->specular = specular;
}


// Getters
vec3 Light::getPosition()
{
	return this->position;
}

vec3 Light::getOrigin()
{
	return this->origin;
}

vec3 Light::getRotation()
{
	return this->rotation;
}

vec3 Light::getScale()
{
	return this->scale;
}

vec3 Light::getAmbient()
{
	return this->ambient;
}

vec3 Light::getDiffuse()
{
	return this->diffuse;
}

vec3 Light::getSpecular()
{
	return this->specular;
}



void Light::move(const vec3 position)
{
	this->position += position;
}

void Light::rotate(const vec3 rotation)
{
	this->rotation += rotation;
}

void Light::scaleUp(const vec3 scale)
{
	this->scale += scale;
}

void Light::render(Shader* shader)
{
	//Update uniforms
	this->updateModelMatrix();
	this->updateUniforms(shader);

	shader->use();

	//Bind VAO
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}