#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
	Camera();

	// matrix which we will produce and eventually eturn
	glm::mat4 getViewMatrix() const; 
	
	void mouseLook(int32_t pMouseX, int32_t pMouseY);
	void moveForward(float pSpeed);
	void moveBackward(float pSpeed);
	void moveLeft(float pSpeed);
	void moveRight(float pSpeed);

private:
	glm::vec3 mEye;
	glm::vec3 mViewDirection;
	glm::vec3 mUpVector;

};

