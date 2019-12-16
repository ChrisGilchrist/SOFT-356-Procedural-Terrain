#include "Skybox.h"

Skybox::Skybox()
{
	// Initialise vao
	this->initVAO();

	// Load the cube map with day textures
	dayTexture = loadCubemap(dayFaces);

	// Load the cube map with night textures
	nightTexture = loadCubemap(nightFaces);
} 

void Skybox::initVAO() {
	//Create VAO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->VERTICES.size() * sizeof(float), VERTICES.data(), GL_STATIC_DRAW);

	//SET VERTEXATTRIBPOINTERS
	//Position	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		nullptr
	);
	

	//BIND VAO 0
	glBindVertexArray(0);
}

unsigned int Skybox::loadCubemap(vector<string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, STBI_rgb_alpha);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;	
}

void Skybox::bindTextures(Shader* shader, float deltaTime) {

	time += deltaTime * 1000;
	time = fmod(time, 24000);
	int texture1;
	int texture2;
	float blendFactor = 0.0f;

	if (time >= 0 && time < 5000) {
		texture1 = nightTexture;
		texture2 = nightTexture;
		blendFactor = (time - 0) / (5000 - 0);
	}
	else if (time >= 5000 && time < 8000) {
		texture1 = dayTexture;
		texture2 = dayTexture;
		blendFactor = (time - 5000) / (8000 - 5000);
	}
	else if (time >= 8000 && time < 21000) {
		texture1 = dayTexture;
		texture2 = dayTexture;
		blendFactor = (time - 8000) / (21000 - 8000);
	}
	else {
		texture1 = dayTexture;
		texture2 = nightTexture;
		blendFactor = (time - 21000) / (24000 - 21000);
	}

	// Bind the cube map
	glActiveTexture(GL_TEXTURE0 + texture1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);

	// Bind the cube map
	glActiveTexture(GL_TEXTURE1 + texture2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);

	shader->set1f(blendFactor, "blendFactor");
}


void Skybox::render(Shader* shader, float deltaTime) {

	time += deltaTime * 1000;
	time = std::fmod(time, 24000);
	GLuint texture1;
	GLuint texture2;
	float blendFactor = 0.0;

	if (time >= 0 && time < 5000) {
		texture1 = nightTexture;
		texture2 = nightTexture;
		blendFactor = (time - 0) / (5000 - 0);
	}
	else if (time >= 5000 && time < 8000) {
		texture1 = nightTexture;
		texture2 = dayTexture;
		blendFactor = (time - 5000) / (8000 - 5000);
	}
	else if (time >= 8000 && time < 21000) {
		texture1 = dayTexture;
		texture2 = dayTexture;
		blendFactor = (time - 8000) / (21000 - 8000);
	}
	else {
		texture1 = dayTexture;
		texture2 = nightTexture;
		blendFactor = (time - 21000) / (24000 - 21000);
	}

	// SET THIS TEMP, SO WE CAN SEE INSIDE OF BOX
	glFrontFace(GL_CW);

	//Bind VAO
	glBindVertexArray(this->VAO);

	// Send texture to the shader
	shader->set1i(dayTexture, "skybox");
	shader->set1i(nightTexture, "skybox2");
	shader->set1f(blendFactor, "blendFactor");

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	glDepthRange(1.f, 1.f);
	shader->use();

	// Bind the cube map (Had to hard code the texture + values as else it would not set them correctly) :/
	glActiveTexture(GL_TEXTURE0 + 6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);

	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);

	//bindTextures(shader, deltaTime);

	// Draw it
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthRange(0.f, 1.f);
	glDepthMask(GL_TRUE);

	//Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// SET THIS SO IT IS BACK TO NORMAL
	glFrontFace(GL_CCW);

}