#pragma once
#ifndef OBJLOADER_H
#define OBJLOADER_H
#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Material.h"
#include "MaterialGroup.h"

#include <regex>
#include <algorithm>
#include <tuple>

using namespace glm;

class FileParser
{
private:

	struct VertRef
	{
		VertRef(int v, int vt, int vn) : v(v), vt(vt), vn(vn) { }
		int v, vt, vn;
	};

public:

	// Empty constructor
	FileParser();

	//Loads Mesh Object from an .obj file
	bool processObjFile(string filePath, Mesh*& mesh, string& materialFileName);

	// Loads The mesh materials from the .mtl file
	bool processMtlFile(string filePath, vector < Texture* >& textures, vector < Material* >& materials, int& textureCount);

	// Loads Mesh and Materials for a .dae file (Collada File)
	bool processColladaFile(string filePath, Mesh*& mesh, vector < Texture* >& textures, vector < Material* >& materials, int& textureCount);

	// Take all the information provided and combine to create the model
	void createObject(Mesh*& mesh, vector < Texture* >& textures, vector < Material* >& materials, vector < Model* >& models);

};

#endif
