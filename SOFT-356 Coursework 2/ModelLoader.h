#pragma once

#include "utils.h"

#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Material.h"
#include "MaterialGroup.h"
#include "Shader.h"
#include "FileParser.h"
#include "Light.h"
#include "Terrain.h"

// Used for the clock timing the initialisation of the model
#include <chrono>
#include <limits>
#include <iomanip>
#include <cmath>

//ENUMERATIONS
enum shader_enum { 
	SHADER_CORE_PROGRAM = 0, 
	SHADER_LIGHT_PROGRAM = 1,
	SHADER_TERRAIN_PROGRAM = 2
};

class ModelLoader
{
private:
	//Variables
	//Window
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	//OpenGL Context
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	//Delta time
	float dt;
	float curTime;
	float lastTime;

	//Mouse Input
	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;

	//Camera
	Camera camera;

	//Matrices
	mat4 ViewMatrix;
	vec3 camPosition;
	vec3 worldUp;
	vec3 camFront;

	mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	// ObjectLoader
	FileParser modelParser;

	//Shaders
	vector<Shader*> shaders;

	//Textures
	vector<Texture*> textures;

	//Materials
	vector<Material*> materials;

	//Terrains 
	//vector<Terrain*> terrains;
	Terrain* terrain;

	//Models
	vector<Model*> models;

	//Light
	Light* light;

	// Useful
	int textureCount = -1;
	bool clearScene = false;
	string choice;
	int selectedModel = 0;

	HWND hwnd;

	//Private functions
	void initGLFW();
	void initWindow(
		const char* title,
		bool resizable
	);
	void initGLEW(); //AFTER CONTEXT CREATION!!!
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();

	void initTerrain();

	// Load Model
	bool initModel(string choiceName);

	void initLights();
	void initUniforms();

	void updateUniforms();


public:
	//Constructors / Destructors
	ModelLoader(
		const char* title,
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
		bool resizable
	);
	virtual ~ModelLoader();

	//Accessors
	int getWindowShouldClose();

	//Modifiers
	void setWindowShouldClose();

	//Functions
	void updateDt();
	void updateMouseInput();
	void updateKeyboardInput();
	void updateInput();
	void update();
	void clearModelInfo();
	void ClearScene();
	void LoadNewObj();
	void render();

	// Model Loader Related Controls
	void displayMenu();
	string getInput();

	//Static functions
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
};