#pragma once

#include "utils.h"

#include"Shader.h"
#include"Camera.h"
#include"Terrain.h"

class MousePicker
{

private:

	int RECURSION_COUNT = 200;
	float RAY_RANGE = 800;

	vec3 currentRay;

	mat4 projectionMatrix;
	mat4 viewMatrix;
	Camera& camera;

	Terrain* terrain;
	vec3 currentTerrainPoint;

	float screenWidth = 1920;
	float screenHeight = 1080;


public:

	MousePicker(Camera& cam);

	vec3 getCurrentRay();

	vec3 getCurrentTerrainPoint();

	void update(GLFWwindow* window);

	vec3 calculateMouseRay(GLFWwindow* window);

	vec4 toEyeCoords(vec4 clipCoords);

	vec3 toWorldCoords(vec4 eyeCoords);

	vec2 getNormalisedDeviceCoord(float mouseX, float mouseY, GLFWwindow* window);


	void setProjection(mat4& projection);

	void setViewMatrix(mat4& view);

	void setTerrain(Terrain* terrain);



	// Terrain stuff
	vec3 getPointOnRay(vec3 ray, float distance);

	vec3 binarySearch(int count, float star, float finish, vec3 ray);

	bool intersectionInRange(float start, float finish, vec3 ray);

	bool isUnderGround(vec3 testPoint);

	Terrain* getTerrain(float worldX, float worldY);

};
