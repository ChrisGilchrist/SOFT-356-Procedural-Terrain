#pragma once

#include "utils.h"

#include "Model.h"

enum direction { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class Camera
{
private:
	mat4 ViewMatrix;

	Model* model;

	float distanceFromPlayer = 50;
	float angleAroundPlayer = 0;

	GLfloat movementSpeed;
	GLfloat sensitivity;

	vec3 worldUp;
	vec3 position;
	vec3 front;
	vec3 right;
	vec3 up;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	// Update the camera vectors
	void updateCameraVectors();

public:
	Camera(vec3 position, vec3 direction, vec3 worldUp);

	~Camera();

	const mat4 getViewMatrix();

	const vec3 getPosition() const;

	float getPitch();

	float getYaw();


	// PLayer stuff
	void calculateCameraPosition(float horDistance, float vertDistance);

	float calculateHorizontalDistance();

	float calculateVerticalDistance();
	
	void setModel(Model* model);

	Model* getModel();



	// Update position vector - Move camera in the direction provided
	void move();

	// Update mouse input - updates the pitch, yaw and roll values
	void updateMouseInput(const float& dt, const double& offsetX, const double& offsetY);

	// Upate Input - calls update mouse input
	void updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY);

};
