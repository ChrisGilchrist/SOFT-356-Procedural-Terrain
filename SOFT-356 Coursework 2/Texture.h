#pragma once

#include "utils.h"

class Texture
{
private:
	GLuint id;
	int width;
	int height;
	unsigned int type;

public:
	Texture();

	Texture(const char* fileName, GLenum type);

	~Texture();

	inline GLuint getID();

	void bind(const GLint texture_unit);

	void unbind();
};