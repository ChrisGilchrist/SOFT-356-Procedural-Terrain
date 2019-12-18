#include "MousePicker.h"


MousePicker::MousePicker(Camera& cam)
	: camera(cam), currentRay(0, 0, 0)
{
}

vec3 MousePicker::getCurrentRay() {
	return currentRay;
}

vec3 MousePicker::getCurrentTerrainPoint() {
	return currentTerrainPoint;
}

void MousePicker::update(GLFWwindow* window) {

	// Create new view matrix
	//glm::mat4 view_matrix{};

	viewMatrix = camera.getViewMatrix();

	currentRay = calculateMouseRay(window);

	if (intersectionInRange(0, RAY_RANGE, currentRay)) {
		currentTerrainPoint = binarySearch(0, 0, RAY_RANGE, currentRay);
	}
	else {
		currentTerrainPoint = vec3(0,0,0);
	}
}

vec3 MousePicker::calculateMouseRay(GLFWwindow* window) {

	double mouseX;
	double mouseY;
	// Get location of mouse (X and Y)
	glfwGetCursorPos(window, &mouseX, &mouseY);

	vec2 normalisedCoords = getNormalisedDeviceCoord(mouseX, mouseY, window);
	vec4 clipCoords = vec4(normalisedCoords.x, normalisedCoords.y, -1.f, 1.f);
	vec4 eyeCoords = toEyeCoords(clipCoords);
	vec3 worldRay = toWorldCoords(eyeCoords);

	return worldRay;
}

vec3 MousePicker::toWorldCoords(vec4 eyeCoords) {

	mat4 invertedView = inverse(viewMatrix);
	vec4 rayWorld = invertedView * eyeCoords;
	vec3 mouseRay = vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	mouseRay = glm::normalize(mouseRay);
	return mouseRay;

}

vec4 MousePicker::toEyeCoords(vec4 clipCoords) {
	glm::vec4 eyeCoords = glm::inverse(projectionMatrix) * clipCoords;
	return { eyeCoords.x, eyeCoords.y, -1.0f, 0.0f };
}

vec2 MousePicker::getNormalisedDeviceCoord(float mouseX, float mouseY, GLFWwindow* window) {

	int* width = new int(0);
	int* height = new int(0);
	glfwGetWindowSize(window, width, height);

	screenWidth = *width;
	screenHeight = *height;

	float x = (2.f * mouseX) / screenWidth - 1.f;
	float y = (2.f * mouseY) / screenHeight - 1.f;
	return vec2(x, -y);
}


void MousePicker::setProjection(mat4& projection) {
	this->projectionMatrix = projection;
}

void MousePicker::setViewMatrix(mat4& view) {
	this->viewMatrix = view;
}

void MousePicker::setTerrain(Terrain* terrain) {
	this->terrain = terrain;
}



/* Terrain stuff, works out where we hit the terrain with the mouse (terrain position) */


vec3 MousePicker::getPointOnRay(vec3 ray, float distance) {
	vec3 camPos = camera.getPosition();
	vec3 start = vec3(camPos.x, camPos.y, camPos.z);
	vec3 scaledRay = vec3(ray.x * distance, ray.y * distance, ray.z * distance);

	vec3 result = vec3(start + scaledRay);

	return result;
}

vec3 MousePicker::binarySearch(int count, float start, float finish, vec3 ray) {
	float half = start + ((finish - start) / 2.f);
	if (count >= RECURSION_COUNT) {
		vec3 endPoint = getPointOnRay(ray, half);
		Terrain* terrain = getTerrain(endPoint.x, endPoint.z);
		if (terrain != nullptr) {
			return endPoint;
		}
		else {
			return vec3(0,0,0);
		}
	}
	if (intersectionInRange(start, half, ray)) {
		return binarySearch(count + 1, start, half, ray);
	}
	else {
		return binarySearch(count + 1, half, finish, ray);
	}
}

bool MousePicker::intersectionInRange(float start, float finish, vec3 ray) {
	vec3 startPoint = getPointOnRay(ray, start);
	vec3 endPoint = getPointOnRay(ray, finish);
	if (!isUnderGround(startPoint) && isUnderGround(endPoint)) {
		return true;
	}
	else {
		return false;
	}
}

bool MousePicker::isUnderGround(vec3 testPoint) {
	Terrain* terrain = getTerrain(testPoint.x, testPoint.z);
	float height = 0;

	if (terrain != nullptr) {
		height = terrain->getHeightOfTerrain(testPoint.x, testPoint.z);
	}
	
	if (testPoint.y < height) {
		return true;
	}
	else {
		return false;
	}
}

Terrain* MousePicker::getTerrain(float worldX, float worldZ) {
	return terrain;
}