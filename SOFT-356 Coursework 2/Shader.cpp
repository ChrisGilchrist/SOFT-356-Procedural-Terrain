#include "Shader.h"

// Private 
string Shader::loadShaderSource(const char* fileName)
{
	string temp = "";
	string src = "";

	ifstream in_file;

	in_file.open(fileName);

	if (in_file.is_open())
	{
		while (getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{
		cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << fileName << "\n";
	}

	in_file.close();

	string versionNr =
		to_string(this->
			versionMajor) +
		to_string(this->
			versionMinor) +
		"0";

	src.replace(src.find("#version"), 12, ("#version " + versionNr));

	return src;
}

GLuint Shader::loadShader(GLenum type, const char* fileName)
{
	char infoLog[512];
	GLint success;

	GLuint shader = glCreateShader(type);
	string str_src = this->loadShaderSource(fileName);
	const GLchar* src = str_src.c_str();
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
		cout << infoLog << "\n";
	}

	return shader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	char infoLog[512];
	GLint success;

	this->id = glCreateProgram();

	glAttachShader(this->id, vertexShader);

	glAttachShader(this->id, fragmentShader);

	glLinkProgram(this->id);

	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->id, 512, NULL, infoLog);
		cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
		cout << infoLog << "\n";
	}

	glUseProgram(0);
}

// Public
Shader::Shader(const int versionMajor, const int versionMinor,
	const char* vertexFile, const char* fragmentFile)
	: versionMajor(versionMajor), versionMinor(versionMinor)
{
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;

	vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);

	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

	this->linkProgram(vertexShader, fragmentShader);

	//End
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(this->id);
}

void Shader::use()
{
	glUseProgram(this->id);
}

void Shader::unuse()
{
	glUseProgram(0);
}


//Set uniform functions
void Shader::set1i(GLint value, const GLchar* name)
{
	this->use();

	glUniform1i(glGetUniformLocation(this->id, name), value);

	this->unuse();
}

void Shader::set1f(GLfloat value, const GLchar* name)
{
	this->use();

	glUniform1f(glGetUniformLocation(this->id, name), value);

	this->unuse();
}

void Shader::setVec2f(fvec2 value, const GLchar* name)
{
	this->use();

	glUniform2fv(glGetUniformLocation(this->id, name), 1, value_ptr(value));

	this->unuse();
}

void Shader::setVec3f(fvec3 value, const GLchar* name)
{
	this->use();

	glUniform3fv(glGetUniformLocation(this->id, name), 1, value_ptr(value));

	this->unuse();
}

void Shader::setVec4f(fvec4 value, const GLchar* name)
{
	this->use();

	glUniform4fv(glGetUniformLocation(this->id, name), 1, value_ptr(value));

	this->unuse();
}

void Shader::setMat3fv(mat3 value, const GLchar* name, GLboolean transpose)
{
	this->use();

	glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, value_ptr(value));

	this->unuse();
}

void Shader::setMat4fv(mat4 value, const GLchar* name, GLboolean transpose)
{
	this->use();

	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, value_ptr(value));

	this->unuse();
}