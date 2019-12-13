#include "Skybox.h"

Skybox::Skybox()
{
	// Initialise these things
	this->initVAO();
	//unsigned int cubemapTexture = loadCubemap(faces);
} 

void Skybox::initVAO() {
	//Create VAO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, this->VERTICES.size() * sizeof(float), VERTICES.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->VERTICES), &VERTICES, GL_STATIC_DRAW);

	//SET VERTEXATTRIBPOINTERS
	//Position	
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float), 
		(void*)0
		//0,
		//nullptr
	);
	glEnableVertexAttribArray(0);

	//BIND VAO 0
	glBindVertexArray(0);
}

unsigned int Skybox::loadCubemap(vector<string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			cout << "Cubemap texture failed to load at path: " << faces[i] << endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

void Skybox::render(Shader* shader) {

	//Bind VAO
	glBindVertexArray(this->VAO);

	// Send texture to the shader
	shader->set1i(0, "skybox");

	glDepthMask(GL_FALSE);
	glDepthRange(1.f, 1.f);
	shader->use();

	// Bind the cube map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Draw it
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthRange(0.f, 1.f);
	glDepthMask(GL_TRUE);

	//Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}