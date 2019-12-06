#include "Material.h"

// Public
Material::Material(
	vec3 ambient,
	vec3 diffuse,
	vec3 specular,
	float opacity,
	float shininess,
	GLint diffuseTex,
	GLint specularTex,
	GLint reflectionTex,
	GLint bumpTex,
	string name,
	float isDiffuseTex,
	float isSpecularTex,
	float isBumpTex
)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->opacity = opacity;
	this->shininess = shininess;
	this->diffuseTex = diffuseTex;
	this->specularTex = specularTex;
	this->reflectionTex = reflectionTex;
	this->bumpTex = bumpTex;
	this->name = name;
	this->isDiffuseTex = isDiffuseTex;
	this->isSpecularTex = isSpecularTex;
	this->isBumpTex = isBumpTex;
}

Material::~Material()
{

}

void Material::sendToShader(Shader& program)
{
	program.setVec3f(this->ambient, "material.ambient");
	program.setVec3f(this->diffuse, "material.diffuse");
	program.setVec3f(this->specular, "material.specular");
	program.set1f(this->opacity, "material.opacity");
	program.set1f(this->shininess, "material.shininess");
	program.set1i(this->diffuseTex, "material.diffuseTex");
	program.set1i(this->specularTex, "material.specularTex");
	program.set1i(this->reflectionTex, "material.reflectionTex");
	program.set1i(this->bumpTex, "material.bumpTex");

	program.set1i(this->isDiffuseTex, "material.isDiffuseTex");
	program.set1i(this->isSpecularTex, "material.isSpecularTex");
	program.set1i(this->isBumpTex, "material.isBumpTex");
}


// Getters
string Material::getName()
{
	return this->name;
}

GLuint Material::getTextureDifId()
{
	return this->diffuseTex;
}

GLuint Material::getTextureSpecId()
{
	return this->specularTex;
}

GLuint Material::getBumpTexId()
{
	return this->bumpTex;
}