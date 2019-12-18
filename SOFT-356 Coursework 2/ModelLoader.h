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
#include "Skybox.h"
#include "MousePicker.h"

// Used for the clock timing the initialisation of the model
#include <chrono>
#include <limits>
#include <iomanip>
#include <cmath>

//ENUMERATIONS
enum shader_enum { 
	SHADER_CORE_PROGRAM = 0, 
	SHADER_LIGHT_PROGRAM = 1,
	SHADER_TERRAIN_PROGRAM = 2,
	SHADER_SKYBOX_PROGRAM = 3
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

	MousePicker mousePicker;

	// ObjectLoader
	FileParser modelParser;

	//Shaders
	vector<Shader*> shaders;

	//Textures
	vector<Texture*> textures;

	//Materials
	vector<Material*> materials;

	//Skybox
	Skybox* skybox;

	//Terrain
	Terrain* terrain;

	//Models
	vector<Model*> models;

	//Light (Sun)
	Light* light;

	// Useful
	int textureCount = -1;
	int selectedModel = 0;
	bool creativeMode = false;

	HWND hwnd;

	// Skybox variables
	float ROTATE_SPEED = 1.f;
	float currentRotation = 0;

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

	void initSkybox();

	void initTerrain();

	void initWorldModels();

	// Load Player Model
	void initPlayer();

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
	void updateKeyboardInput(float delta);
	void updateInput();
	void update();
	void clearModelInfo();
	void render();

	// Start Screen
	void displayMenu();

	//Static functions
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
};