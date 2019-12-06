#pragma once

#include "utils.h";

class Shader
{
private:

	GLuint id;
	const int versionMajor;
	const int versionMinor;

	string loadShaderSource(const char* fileName);

	GLuint loadShader(GLenum type, const char* fileName);

	void linkProgram(GLuint vertexShader, GLuint fragmentShader);

public:

	//Constructors/Destructors
	Shader(const int versionMajor, const int versionMinor,
		const char* vertexFile, const char* fragmentFile);

	~Shader();

	void use();

	void unuse();

	//Set uniform functions
	void set1i(GLint value, const GLchar* name);

	void set1f(GLfloat value, const GLchar* name);

	void setVec2f(fvec2 value, const GLchar* name);

	void setVec3f(fvec3 value, const GLchar* name);

	void setVec4f(fvec4 value, const GLchar* name);

	void setMat3fv(mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE);

	void setMat4fv(mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);

};