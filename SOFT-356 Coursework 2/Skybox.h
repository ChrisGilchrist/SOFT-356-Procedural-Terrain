#pragma once

#include "utils.h"

#include "stb_image.h"
#include "Shader.h"

class Skybox
{
private:

	unsigned int dayTexture;
	unsigned int nightTexture;

	vector<std::string> dayFaces
	{
		"skybox/right.png",
		"skybox/left.png",
		"skybox/top.png",
		"skybox/bottom.png",
		"skybox/back.png",
		"skybox/front.png"
	};

	vector<std::string> nightFaces
	{
		"skybox/nightRight.png",
		"skybox/nightLeft.png",
		"skybox/nightTop.png",
		"skybox/nightBottom.png",
		"skybox/nightBack.png",
		"skybox/nightFront.png"
	};

	float SIZE = 300.0f;

	float time = 0;

	//float blendFactor = 0.67434235f;

	vector<float> VERTICES = {
		-SIZE,  SIZE, -SIZE,
		-SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		-SIZE,  SIZE, -SIZE,
		 SIZE,  SIZE, -SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE
	};

	vec3 position;
	vec3 origin;
	vec3 rotation;
	vec3 scale;

	GLuint VAO;
	GLuint vertexBuffer;

	void initVAO();

public:
	Skybox();

	~Skybox();

	unsigned int loadCubemap(vector<std::string> faces);

	void bindTextures(Shader* shader, float deltaTime);


	// Render the mesh to the scene
	void render(Shader* shader, float deltaTime);

};
