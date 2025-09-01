#include "Camera.h"

Camera::Camera(uint32_t pCameraWidth, uint32_t pCameraHeight, const glm::vec3& pPos,
			   float pFOV, float pNearPlane, float pFarPlane)
{
	mWidthCam = pCameraWidth;
	mHeightCam = pCameraHeight;
	mFOV = pFOV;
	mNearPlane = pNearPlane;
	mFarPlane = pFarPlane;
	mEye = pPos;
	mDirection = { 0.0f,0.0f,-1.0f };
	mUpVector = { 0.0f,1.0f,0.0f };
}

glm::mat4 Camera::getViewMatrix()
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 perspective = glm::mat4(1.0f);

	view = glm::lookAt(mEye, mEye + mDirection, mUpVector);

	perspective = glm::perspective(glm::radians(mFOV), (float)(mWidthCam / mHeightCam), mNearPlane, mFarPlane);

	mCameraMatrix = view * perspective;
	return mCameraMatrix;
}

void Camera::matrix(Shader pShader, std::string_view pName)
{
	pShader.setUniform4fv(pName, mCameraMatrix);
}

void Camera::moveMouse(const glm::vec2 pMousePos)
{
	mDirection = glm::rotate(mDirection, glm::radians(pMousePos.x), mUpVector);

	glm::vec3 xVec3 = glm::normalize(glm::cross(mDirection, mUpVector));
	mDirection = glm::rotate(mDirection, glm::radians(pMousePos.y), xVec3);
}

void Camera::moveRight(float pSpeed)
{
	glm::vec3 tmpLeftVec = glm::normalize(glm::cross(mDirection, mUpVector));
	mEye += tmpLeftVec * pSpeed;
}

void Camera::moveLeft(float pSpeed)
{
	glm::vec3 tmpLeftVec = glm::normalize(glm::cross(mUpVector, mDirection));
	mEye += tmpLeftVec * pSpeed;
}

void Camera::moveForward(float pSpeed)
{
	mEye += mDirection * pSpeed;
}

void Camera::moveBackward(float pSpeed)
{
	mEye -= mDirection * pSpeed;
}

void Camera::setCameraWidth(uint32_t pCameraWidth)
{
	mWidthCam = pCameraWidth;
}

void Camera::setCameraHeight(uint32_t pCameraHeight)
{
	mHeightCam = pCameraHeight;
}

void Camera::setCameraSize(uint32_t pCameraWidth, uint32_t pCameraHeight)
{
	mWidthCam = pCameraWidth;
	mHeightCam = pCameraHeight;
}

void Camera::setFOV(float pFOV)
{
	mFOV = pFOV;
}

void Camera::setNearPlane(float pNearPlane)
{
	mNearPlane = pNearPlane;
}

void Camera::setFarPlane(float pFarPlane)
{
	mFarPlane = pFarPlane;
}

glm::vec3 Camera::getPosition() const noexcept
{
	return mEye;
}

glm::vec3 Camera::getDirection() const noexcept
{
	return mDirection;
}

glm::vec3 Camera::getUpVector() const noexcept
{
	return mUpVector;
}

glm::mat4 Camera::getCameraMatrix() const noexcept
{
	return mCameraMatrix;
}

uint32_t Camera::getWidthCamera() const noexcept
{
	return mWidthCam;
}

uint32_t Camera::getHeightCamera() const noexcept
{
	return mHeightCam;
}

uint32_t Camera::getFOV() const noexcept
{
	return mFOV;
}

uint32_t Camera::getNearPlane() const noexcept
{
	return mNearPlane;
}

uint32_t Camera::getFarPlane() const noexcept
{
	return mFarPlane;
}
