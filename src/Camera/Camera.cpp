#include "Camera.h"

Camera::Camera()
{
	// assume, we are placed at the origin
	mEye = glm::vec3(0.0f, 0.0f, 0.0f);
	// assume we are looking out into the world
	// NOTE: the negative value is set, because objects
	//		 which are in a negative value - in front of us, 
	//		 otherwise (positive) - behind
	mViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	// assume we start on a perfect plane
	mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(mEye, mViewDirection, mUpVector);
}

void Camera::moveForward(float pSpeed)
{
	mEye.z += pSpeed;
	
}

void Camera::moveBackward(float pSpeed)
{
	mEye.z -= pSpeed;
}

void Camera::moveLeft(float pSpeed)
{
	mEye.x += pSpeed;
}

void Camera::moveRight(float pSpeed)
{
	mEye.x -= pSpeed;
}
