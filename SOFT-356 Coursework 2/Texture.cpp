#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// Public functions
Texture::Texture(const char* fileName, GLenum type)
{
	this->type = type;
	unsigned char* image = stbi_load(fileName, &this->width, &this->height, NULL, STBI_rgb_alpha);

	glGenTextures(1, &this->id);
	glBindTexture(type, this->id);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image)
	{
		//cout << "SUCCESS::TEXTURE::TEXTURE_LOADING_WORKED: " << fileName << "\n";
		glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(type);
	}
	else
	{
		//cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
	}

	//cout << "SUCCESS::TEXTURE::TEXTURE_HAS_BEEN_BINDED: " << fileName << "\n";
	glActiveTexture(0);
	glBindTexture(type, 0);
	stbi_image_free(image);
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->id);
}

inline GLuint Texture::getID()
{
	return this->id;
}

void Texture::bind(const GLint texture_unit)
{
	//cout << "SUCCESS::TEXTURE::TEXTURE_HAS_BEEN_BINDED: " << texture_unit << "\n";
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(this->type, this->id);
}

void Texture::unbind()
{
	glActiveTexture(0);
	glBindTexture(this->type, 0);
}
