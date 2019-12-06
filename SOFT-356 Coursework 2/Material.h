#pragma once

#include "utils.h"

#include"Shader.h"

class Material
{
private:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float opacity;
	float shininess;
	GLint diffuseTex;
	GLint specularTex;
	GLint reflectionTex;
	GLint bumpTex;
	string name;

	// These are used to indicate whether there is a texture defined or not
	float isDiffuseTex;
	float isSpecularTex;
	float isBumpTex;

public:

	Material(
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
	);

	~Material();

	// Send the material information to the shader
	void sendToShader(Shader& program);

	// Returns the name of the material
	string getName();

	// Returns the id for the diffuse texture
	GLuint getTextureDifId();

	//Returns the id for the specular texture
	GLuint getTextureSpecId();

	// Returns the id for the bump texture
	GLuint getBumpTexId();
};