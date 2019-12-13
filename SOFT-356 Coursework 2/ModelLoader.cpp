#include "ModelLoader.h"

using namespace std;
using namespace glm;

struct TextureInfo {
	const char* texName;
	const char* filePath;
};

//Private functions
void ModelLoader::initGLFW()
{
	//INIT GLFW
	if (!glfwInit())
	{
		cout << "ERROR::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void ModelLoader::initWindow(
	const char* title,
	bool resizable
)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr)
	{
		cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	glfwSetFramebufferSizeCallback(window, ModelLoader::framebuffer_resize_callback);
	glfwMakeContextCurrent(this->window);	
}

void ModelLoader::initGLEW()
{
	//INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	// Catch error if something goes wrong when initialising Glew 
	if (glewInit() != GLEW_OK)
	{
		cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void ModelLoader::initOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE); // TODO - Remove these, These are causing the issue with transparent parts of the model 
	//glCullFace(GL_BACK); // TODO - Remove these These are causing the issue with transparent parts of the model 
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // <!--- This shows the model in a wireframe view

	//Input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ModelLoader::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	// Deals with the resize of the screen
	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);
}

void ModelLoader::initShaders()
{
	// Shader files for the models
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		"shaders/vertex_core.glsl", "shaders/fragment_core.glsl"));

	// Shader files for the light
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		"shaders/light_vertex_core.glsl", "shaders/light_fragment_core.glsl"));

	// Sahder files for the terrain
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		"shaders/terrain_vertex_core.glsl", "shaders/terrain_fragment_core.glsl"));

	// Sahder files for the skybox
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		"shaders/skybox_vertex_core.glsl", "shaders/skybox_fragment_core.glsl"));
}

void ModelLoader::initSkybox()
{
	skybox = new Skybox();

	cout << "Skybox is ready \n";
}

void ModelLoader::initTerrain()
{
	// Create new terrain
	terrain = new Terrain(
		0,
		0,
		vec3(0.f, 0.f, 2.f),
		vec3(0.f),
		vec3(0, 0, 0),
		vec3(1.f)
	);

	cout << "Terrain is ready \n";
}

bool ModelLoader::initModel(string choiceName)
{
	bool fileLoadedCorrectly = false;

	// Init array of meshes
	Mesh* mesh;

	fileLoadedCorrectly = modelParser.processColladaFile("models/" + choiceName, mesh, textures, materials, textureCount);

	// Now take all the materials and meshes to load in a object.
	modelParser.createObject(mesh, textures, materials, models);

	cout << "Model is ready! \n";

	// Clear the mesh once done with it
	delete mesh;

	// Once this is done, tell the camera that is the model to follow
	this->camera.setModel(models[0]);

	return fileLoadedCorrectly;
}

void ModelLoader::initLights()
{
	// Create light used to illuminate the object
	light = new Light(
		//vec3(10.f, 5.f, -1.f), // Closer to models
		vec3(20.f, 10.f, -1.f),
		vec3(0.f),
		vec3(0, 0, 0),
		vec3(0.5f),
		vec3(0.8f, 0.8f, 0.8f),
		vec3(0.8f, 0.8f, 0.8f),
		vec3(1.0f, 1.0f, 1.0f)
	);
}

void ModelLoader::initUniforms()
{
	//INIT UNIFORMS
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->light->getPosition(), "lightPos0");

	this->shaders[SHADER_LIGHT_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_LIGHT_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	this->shaders[SHADER_TERRAIN_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_TERRAIN_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	this->shaders[SHADER_SKYBOX_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_SKYBOX_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->light->getAmbient(), "lightAmbient");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->light->getDiffuse(), "lightDiffuse");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->light->getSpecular(), "lightSpecular");
}

void ModelLoader::updateUniforms()
{
	//Update view matrix (camera)
	this->ViewMatrix = this->camera.getViewMatrix();

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos");

	this->shaders[SHADER_LIGHT_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");

	this->shaders[SHADER_TERRAIN_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");

	this->shaders[SHADER_SKYBOX_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");

	//Update framebuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
	this->shaders[SHADER_LIGHT_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
	this->shaders[SHADER_TERRAIN_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
	this->shaders[SHADER_SKYBOX_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

//Constructors / Destructors
ModelLoader::ModelLoader(
	const char* title,
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	bool resizable
)
	:
	WINDOW_WIDTH(WINDOW_WIDTH),
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	GL_VERSION_MAJOR(GL_VERSION_MAJOR),
	GL_VERSION_MINOR(GL_VERSION_MINOR),
	camera(glm::vec3(0.f, 100.f, 0.f), glm::vec3(0.f, 10.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
	//camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
{
	bool fileLoaded = false;

	//Init variables
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;

	this->camPosition = glm::vec3(0.f, 0.f, 0.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->dt = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();

	this->initMatrices();
	this->initShaders();

	this->initSkybox();

	this->initTerrain();

	fileLoaded = this->initModel("creeper.dae");
	fileLoaded = true;
	// If it is not true then we ask for them to choose another file
	if (!fileLoaded)
	{
		ClearScene();
		LoadNewObj();
	}

	this->initLights();
	this->initUniforms();

	// Get the console window for later use
	this->hwnd = GetConsoleWindow();
}

ModelLoader::~ModelLoader()
{
	// Destroy the windows and all the other parts of the program
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for (auto*& i : this->models)
		delete i;

	//delete skybox;

	delete light;

	delete terrain;
}

//Accessor
int ModelLoader::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

//Modifier
void ModelLoader::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GL_TRUE);
}

//Functions
void ModelLoader::updateDt()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

void ModelLoader::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	//Calc offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	//Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}

void ModelLoader::updateKeyboardInput(float delta)
{
	//Program Controls
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, GL_TRUE);
	}
	if (glfwGetKey(this->window, GLFW_KEY_K) == GLFW_PRESS)
	{
		ClearScene();
	}
	if (glfwGetKey(this->window, GLFW_KEY_L) == GLFW_PRESS)
	{
		LoadNewObj();
	}


	// Move Player
	if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		float terrainHeight = terrain->getHeightOfTerrain(
			models[selectedModel]->getPositionX(),
			models[selectedModel]->getPositionZ());

		Model* model = models[selectedModel];

		model->setPositionY(terrainHeight);
		

		// Tell mesh we are going forwards
		models[selectedModel]->move("FORWARD", dt);

	}

	if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		float terrainHeight = terrain->getHeightOfTerrain(
			models[selectedModel]->getPositionX(),
			models[selectedModel]->getPositionZ());

		Model* model = models[selectedModel];

		model->setPositionY(terrainHeight);

		models[selectedModel]->move("BACKWARDS", dt);
	}

	if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{

		//this->camera.move();

		float terrainHeight = terrain->getHeightOfTerrain(
			models[selectedModel]->getPositionX(),
			models[selectedModel]->getPositionZ());

		Model* model = models[selectedModel];

		model->setPositionY(terrainHeight);
		
		models[selectedModel]->move("TURN_LEFT", dt);
	}

	if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
	
		//this->camera.move();

		float terrainHeight = terrain->getHeightOfTerrain(
			models[selectedModel]->getPositionX(),
			models[selectedModel]->getPositionZ());

		Model* model = models[selectedModel];

		model->setPositionY(terrainHeight);
		
		models[selectedModel]->move("TURN_RIGHT", dt);
	}

	/* Scale up
	if (glfwGetKey(this->window, GLFW_KEY_B) == GLFW_PRESS)
	{
		models[selectedModel]->scale(vec3(0.001f));
	}

	// Scale Down
	if (glfwGetKey(this->window, GLFW_KEY_N) == GLFW_PRESS)
	{
		models[selectedModel]->scale(vec3(-0.001f));
	}

	// Rotate Model left
	if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		models[selectedModel]->rotate(vec3(0, 0.5f, 0));
	}

	// Rotate Model right
	if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS)
	{
		models[selectedModel]->rotate(vec3(0, -0.5f, 0));
	}

	// Change selected model - Switches the next model in the list
	if (glfwGetKey(this->window, GLFW_KEY_M) == GLFW_PRESS)
	{
		int size = models.size();

		// If the selected model isn't the same as the size
		if (selectedModel != size - 1) {
			selectedModel += 1;
		}
		else
		{
			// Set it back to 0
			selectedModel = 0;
		}
	}

	*/
}

void ModelLoader::updateInput()
{
	glfwPollEvents();

	this->updateKeyboardInput(dt);
	this->updateMouseInput();

	/*
	if (glfwGetKey(this->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{

	}
	*/

	this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
	this->camera.move();
	
}

void ModelLoader::update()
{
	//UPDATE INPUT ---
	this->updateDt();
	this->updateInput();
}

// Cleans up the memory, remove all the info stored about the old model
void ModelLoader::clearModelInfo()
{
	// Clear the arrays storing model info
	textures.clear();
	materials.clear();
	models.clear();

	delete terrain;

	// Reset texture count to -1
	textureCount = -1;
}

void ModelLoader::ClearScene()
{
	// Make sure to reset the selected Model
	selectedModel = 0;
	
	clearModelInfo();

	// Tell the renderer not to do anything until this is false
	this->clearScene = true;
}

void ModelLoader::LoadNewObj()
{
	bool fileCorrect = false;	

	// Bring the console to the front so they can choose from the list
	//ShowWindow(hwnd, SW_NORMAL);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	SetForegroundWindow(hwnd);

	// Ask the user for a new model
	cout << "\n";
	cout << "Please enter the name of the file to load:\n";
	choice = getInput();

	this->clearScene = false;

	fileCorrect = this->initModel(choice);

	if (!fileCorrect)
	{
		// try again
		LoadNewObj();
	}
}


void ModelLoader::render()
{

	//DRAW ---
	//Clear
	//glClearColor(0.3f, 0.3f, 0.3f, 1);
	glClearColor(0.4f, 0.3f, 0.6f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update the uniforms
	this->updateUniforms();

	// Render Skybox
	this->skybox->render(this->shaders[SHADER_SKYBOX_PROGRAM]);

	// Render Terrain
	this->terrain->render(this->shaders[SHADER_TERRAIN_PROGRAM]);

	// Render lamp - stays in same regardless
	this->light->render(this->shaders[SHADER_LIGHT_PROGRAM]);

	// If clear scene is true then dont render the objects to scene
	if (!clearScene) {
		//Render models
		for (auto& i : this->models)
			i->render(this->shaders[SHADER_CORE_PROGRAM], this->textures);
	}

	//End Draw
	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


// New game related controls
void ModelLoader::displayMenu()
{
	cout << "\n";
	cout << "\n";
	cout << "#######################################\n";
	cout << "### WELCOME TO CHRIS'S MODEL LOADER ###\n";
	cout << "#######################################\n";
	cout << "\n";
	cout << "Game controls:\n";
	cout << "Move mouse to control the camera view\n";
	cout << "Press (W, A, S, D) to move camera around\n";
	cout << "Press (SPACE) to fly up\n";
	cout << "Press (C) to fly down\n";
	cout << "Press (Q) to rotate model left\n";
	cout << "Press (E) to rotate model right\n";
	cout << "Press (B) to increase scale\n";
	cout << "Press (N) to decrease scale\n";
	cout << "\n";
	cout << "Program controls:\n";
	cout << "Press (Q or ESC) to quit\n";
	cout << "Press (K) to clear scene\n";
	cout << "Press (L) to load new Object in\n";
	cout << "Press (M) to toggle between the objects in the scene (Individual Control)\n";
	cout << "\n";
	cout << "Please enter the name of the file to load:\n";

}

// Gets the name of the file that the user inputs
string ModelLoader::getInput()
{
	string choice;
	getline(cin, choice);
	cout << "Loading " << choice << "... \n";
	return choice;
}

//Static functions
void ModelLoader::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
};

