#pragma once

#include "Camera.h"

/* Private 
void Camera::updateCameraVectors() {
	this->front.x = cos(radians(this->yaw)) * cos(radians(this->pitch));
	this->front.y = sin(radians(this->pitch));
	this->front.z = sin(radians(this->yaw)) * cos(radians(this->pitch));
	this->front = normalize(this->front);

	this->right = normalize(cross(this->front, this->worldUp));
	this->up = normalize(cross(this->right, this->front));
}
*/

// Public 
Camera::Camera(vec3 position, vec3 direction, vec3 worldUp)
{
	this->ViewMatrix = mat4(1.f);

	// this->movementSpeed = 3.f;
	this->movementSpeed = 60.f;
	this->sensitivity = 5.f;

	this->worldUp = worldUp;
	this->position = position;
	this->right = vec3(0.f);
	this->up = worldUp;

	this->pitch = 20.f;
	this->yaw = 0.f;
	this->roll = 0.f;

	//this->updateCameraVectors();

}

Camera::~Camera()
{

}

const mat4 Camera::getViewMatrix()
{
	//this->updateCameraVectors();

	// We need to set it so its always looking at the object using its position.
	vec3 objectPosition = vec3(this->model->getPositionX(), this->model->getPositionY(), this->model->getPositionZ());
	this->ViewMatrix = lookAt(position, objectPosition, this->up);

	return this->ViewMatrix;
}

const vec3 Camera::getPosition() const
{
	return this->position;
}


void Camera::calculateCameraPosition(float horDistance, float vertDistance) {
	
	//calc
	float theta = this->model->getRotationY() + angleAroundPlayer;
	float offsetX = (float)(horDistance * sin(radians(theta)));
	float offsetZ = (float)(vertDistance * cos(radians(theta)));
	position.x = this->model->getPositionX() - offsetX;
	position.z = this->model->getPositionZ() - offsetZ;
	position.y = this->model->getPositionY() + vertDistance;

}

float  Camera::calculateHorizontalDistance() {
	//return (float) (distanceFromPlayer * cos(radians(pitch)));
	return distanceFromPlayer;
}

float  Camera::calculateVerticalDistance() {
	//return (float) (distanceFromPlayer * sin(radians(pitch)));
	return distanceFromPlayer;
}

void Camera::setModel(Model* model) {
	this->model = model;
}

Model* Camera::getModel() {
	return this->model;
}


void Camera::move()
{
	float horDistance = calculateHorizontalDistance();
	float vertDistance = calculateVerticalDistance();
	calculateCameraPosition(horDistance, vertDistance);

	yaw = 180 - (model->getRotationY() + angleAroundPlayer);
}


void Camera::updateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
{
	//Update pitch yaw and roll	
	this->pitch += static_cast<GLfloat>(offsetY)* this->sensitivity* dt;
	this->yaw += static_cast<GLfloat>(offsetX)* this->sensitivity* dt;

	if (this->pitch > 80.f)
		this->pitch = 80.f;
	else if (this->pitch < -80.f)
		this->pitch = -80.f;

	// Reset if it goes above 360
	if (this->yaw > 360.f || this->yaw < -360.f)
		this->yaw = 0.f;
		
}

void Camera::updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
{
	this->updateMouseInput(dt, offsetX, offsetY);
}
