#include "Mesh.h"

// Private
void Mesh::initVAO()
{
	//Create VAO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

	//GEN EBO AND BIND AND SEND DATA
	if (this->nrOfIndices > 0)
	{
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
	}

	//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	//Color
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);*/
	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);
	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	//BIND VAO 0
	glBindVertexArray(0);
}

void Mesh::updateUniforms(Shader* shader)
{
	shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
}

void Mesh::updateModelMatrix()
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
Mesh::Mesh(
	Vertex* vertexArray,
	const unsigned& nrOfVertices,
	GLuint* indexArray,
	const unsigned& nrOfIndices,
	MaterialGroup* materialGroups,
	const unsigned& nrOfMatGroups,
	vec3 position = vec3(0.f),
	vec3 origin = vec3(0.f),
	vec3 rotation = vec3(0.f),
	vec3 scale = vec3(1.f))
{
	this->position = position;
	this->origin = origin;
	this->rotation = rotation;
	this->scale = scale;

	this->nrOfMatGroups = nrOfMatGroups;
	this->nrOfVertices = nrOfVertices;
	this->nrOfIndices = nrOfIndices;

	this->materialGroups = new MaterialGroup[this->nrOfMatGroups];
	for (size_t i = 0; i < nrOfMatGroups; i++)
	{
		this->materialGroups[i] = materialGroups[i];
	}

	this->vertexArray = new Vertex[this->nrOfVertices];
	for (size_t i = 0; i < nrOfVertices; i++)
	{
		this->vertexArray[i] = vertexArray[i];
	}

	this->indexArray = new GLuint[this->nrOfIndices];
	for (size_t i = 0; i < nrOfIndices; i++)
	{
		this->indexArray[i] = indexArray[i];
	}

	this->initVAO();
	this->updateModelMatrix();
}

Mesh::Mesh(const Mesh& obj)
{
	this->position = obj.position;
	this->origin = obj.origin;
	this->rotation = obj.rotation;
	this->scale = obj.scale;

	this->nrOfMatGroups = obj.nrOfMatGroups;
	this->nrOfVertices = obj.nrOfVertices;
	this->nrOfIndices = obj.nrOfIndices;

	this->materialGroups = new MaterialGroup[this->nrOfMatGroups];
	for (size_t i = 0; i < nrOfMatGroups; i++)
	{
		this->materialGroups[i] = obj.materialGroups[i];
	}

	this->vertexArray = new Vertex[this->nrOfVertices];
	for (size_t i = 0; i < this->nrOfVertices; i++)
	{
		this->vertexArray[i] = obj.vertexArray[i];
	}

	this->indexArray = new GLuint[this->nrOfIndices];
	for (size_t i = 0; i < this->nrOfIndices; i++)
	{
		this->indexArray[i] = obj.indexArray[i];
	}

	this->initVAO();
	this->updateModelMatrix();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);

	if (this->nrOfIndices > 0)
	{
		glDeleteBuffers(1, &this->EBO);
	}

	// clear the arrays of information
	delete[] this->materialGroups;
	delete[] this->vertexArray;
	delete[] this->indexArray;
}


void Mesh::setPosition(const vec3 position)
{
	this->position = position;
}

void Mesh::setOrigin(const vec3 origin)
{
	this->origin = origin;
}

void Mesh::setRotation(const vec3 rotation)
{
	this->rotation = rotation;
}

void Mesh::setScale(const vec3 setScale)
{
	this->scale = scale;
}


void Mesh::move(const vec3 position)
{
	this->position += position;
}

void Mesh::rotate(const vec3 rotation)
{
	this->rotation += rotation;
}

void Mesh::scaleUp(const vec3 scale)
{
	this->scale += scale;
}


void Mesh::render(Shader* shader, vector<Material*> materials, vector<Texture*> textures)
{
	//Update uniforms
	this->updateModelMatrix();
	this->updateUniforms(shader);

	//Bind VAO
	glBindVertexArray(this->VAO);

	for (int i = 0; i < nrOfMatGroups; i++)
	{
		for (int j = 0; j < materials.size(); j++)
		{
			string name = materials[j]->getName();
			if (materialGroups[i].materialName == name)
			{
				// send to shader
				materials[j]->sendToShader(*shader);

				shader->use();

				// Bind the materials textures 
				int id = materials[j]->getTextureDifId();
				int id2 = materials[j]->getTextureSpecId();
				int id3 = materials[j]->getBumpTexId();

				if (id >= 0) {
					textures[id]->bind(id);
				}

				if (id2 >= 0) {
					textures[id2]->bind(id2);
				}

				if (id3 >= 0)
				{
					textures[id3]->bind(id3);
				}

				// Then draw 
				for (int x = 0; x < materialGroups[i].indexPairsArray.size(); x++)
				{
					glDrawArrays(GL_TRIANGLES, materialGroups[i].indexPairsArray[x].first, materialGroups[i].indexPairsArray[x].second);
				}

				break;
			}
		}
	}

	//Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}