# SOFT 356 Interactive Prototype

## Video Demo
[![Video Demo](https://github.com/ChrisGilchrist/SOFT-356-Procedural-Terrain/blob/master/SOFT-356%20Coursework%202/textures/Capture.PNG)](https://www.youtube.com/watch?v=VPQkbFIXBiE&feature=youtu.be 
"https://www.youtube.com/watch?v=VPQkbFIXBiE&feature=youtu.be")
Link: https://www.youtube.com/watch?v=VPQkbFIXBiE&feature=youtu.be

## Project Aim
The aim of the project was to create a basic game engine that focused on the PCG when constructing the world in which the 
user interacts with.

## Versions Used
- Visual Studio 2019
- OpenGL 4.6

## Libraries Used
To make the process simple, I used the nuget packet manager in visual studio to install these libraries:
-	GLM for the mathematics 
-	Nupengl.core (FreeGLUT, GLEW and GLFW) used to build the OpenGL application

I also used stb_image, however I could not install it using the package manager. So instead I added the stb_image.h header file to my application.

## Project Content
-	Procedurally generated terrain and height map generation
-	Terrain Collision detection
-	3rd Person Camera
-	Skybox with a day / night cycle
-	Multiple entities rendered into the scene for scenery
-	Player that the user can move around in the world
-	Multi-texturing with the use of a blend map
-	Mouse Pointer functionality (allows user to interact with the world using the mouse)

## How use the prototype

**Important** Make sure that the folders containing textures etc.. are in the same folder as the .exe file as it will look for these when rendering the scene. (Folders: shaders, textures, models, skybox, terrain)

To play the prototype, you can simply run the executable file. Once the application has loaded, 
the user can then interact with the product with the following controls:

### Game Controls
- Press W A S D to move player around
-	Press and Hold Left mouse button to move camera up and down
-	Press and Hold Right mouse button to move camera left and right
-	Press SPACE to jump up and down
-	Press R to reset player's camera view

### Program Controls
-	Q or Esc to quit the application
-	Press and hold L to change to line mode
-	Press M to toggle mouse picker mode on (creative mode)
-	Press N to toggle mouse picker mode off (creative mode)

## How do the classes and functions fit together and who does what?
I have tried to take an OOP approach to make navigating my code as simple as possible. I have created separate classes for all parts of the code to ensure there is very little confusion. Each class has both a header file, which contains the class definitions and functions and a .cpp file which contains the implementation of the class. The main file which contains most of the functionality is the modelLoader.cpp,
this is where all the OpenGL configuration is handled, and the classes are implemented. The main file creates a Model Loader object which initiates the game engine and then calls the render() method to display the results.

As a lot of the classes in the application include the same header files, I decided to create a utils.h which contains the include for generic header files such as GLFW, GLUT, GLM as well as the common files such as iostream and string. This meant that I could then include this one file rather than each separate file each time. 

### New Content that has been implemented
The **terrain** class was created to hold all the information related to the terrain. 

-	Creating the VAO’s and it’s corresponding VBOs 
-	Generation of the terrain 
-	Creation of textures that will be used to colour the terrain
-	Offers a function that returns the height of the terrain at a certain point in the world by x and z coordinates
-	Rendering the terrain to the scene

The **mouse pointer** class handles the functionality which allows the user to interact at a specific coordinate by translating the x and y position of the mouse on the screen into x and z coordinates of the terrain. 

-	Convert mouse cursor from viewport space to a 3D ray that it is in world space
-	Conducts a binary search to find where on the terrain the ray intersects

The **skybox** class which handles the creation of the skybox which encapsulates the world and controls the day / night cycle.

-	Creating the VAO’s and it’s corresponding VBOs
-	Creation of cubemap textures that are used to colour the skybox
-	Binding specific textures to the shader depending on where in the day / night cycle we are at 
-	Rendering the skybox to the scene

The **heights generation** class offers the functionality to create a procedurally generated terrain using the Perlin noise algorithm.

-	Randomly generates the height of the terrain at a specific point (x and z coordinates)
-	Used by the terrain class, when generating the terrain to decide the height at each point. 

The **camera** class handles the creation of a 3rd person camera that the user will use to see where the player is in the world and will be able to follow the player every time they move. I had to rework the exisiting camera from a free roam camera to a 3rd person.

-	Creation of the camera view matrix
-	Offers getters / setters allowing the main loop to interact with the camera
-	Calculates the position of the camera each frame using the players position, ensuring that it is always focusing on the player. (Like in a 3rd person game)

## What makes your program special and how is it compared to similiar things?

### Where did you get the idea from?
During the first piece of coursework, I did a lot of research on OpenGL and its capabilities. A lot of the research I conducted was of projects that people had produced using the OpenGL / C++ combination. There were many great examples of interactive games and simulations ranging in complexity. One which I took the majority of my ideas from, was a Minecraft replica created using C++. For this piece of coursework, I decided that I wanted to take the best bits from all the projects I viewed and incorporate them into one game engine.

### What did you start with?
Due to time constraints and the amount of work I put into creating my model loader, I decided that it would be best to use the model loader code as a base and build off it. This worked out great as it meant that I did not have to start fresh and so could jump straight into researching / working on terrain generation.

### How did you make yours unique?
I made my game engine prototype unique by taking a range of ideas based off current retail games which I play regularly, as well as the products that I found through research and combined them into my own creation.


