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
	return glm::lookAt(mEye, mEye + mViewDirection, mUpVector);
}

void Camera::mouseLook(float pMouseX, float pMouseY)
{
	glm::vec2 currentMouse = glm::vec2(pMouseX, pMouseY);
	
	static bool firstLook = true;
	if (firstLook)
	{
		mOldMousePos = currentMouse;
		firstLook = false;
	}

	glm::vec2 mouseDelta = mOldMousePos - currentMouse;

	mViewDirection = glm::rotate(mViewDirection, glm::radians(mouseDelta.x), mUpVector);

	glm::vec3 xVector3 = glm::normalize(glm::cross(mViewDirection, mUpVector));
	mViewDirection = glm::rotate(mViewDirection, glm::radians(mouseDelta.y), xVector3);

	mOldMousePos = currentMouse;
}

void Camera::moveForward(float pSpeed)
{
	mEye += (mViewDirection * pSpeed);
}	

void Camera::moveBackward(float pSpeed)
{
	mEye -= (mViewDirection * pSpeed);
}

void Camera::moveLeft(float pSpeed)
{
	// cross product is finding an orthogonal vector to Z and Y axis
	// normalizing is making moving a bit smoother
	glm::vec3 tmpLeftVec = glm::normalize(glm::cross(mUpVector, mViewDirection));
	mEye += tmpLeftVec * pSpeed;
}

void Camera::moveRight(float pSpeed)
{
	glm::vec3 tmpRightVec = glm::normalize(glm::cross(mViewDirection, mUpVector));
	mEye += tmpRightVec * pSpeed;
}
