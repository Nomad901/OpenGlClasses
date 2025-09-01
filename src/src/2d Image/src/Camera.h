#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

class Camera
{
public:
	Camera();

	// matrix which we will produce and eventually eturn
	glm::mat4 getViewMatrix() const; 
	
	void mouseLook(float pMouseX, float pMouseY);
	void moveForward(float pSpeed);
	void moveBackward(float pSpeed);
	void moveLeft(float pSpeed);
	void moveRight(float pSpeed);

private:
	glm::vec3 mEye{};
	glm::vec3 mViewDirection{};
	glm::vec3 mUpVector{};

	glm::vec2 mOldMousePos{};

};

