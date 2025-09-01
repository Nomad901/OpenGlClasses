#pragma once
#include <filesystem>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "Shader.h"

class Camera
{
public:
	Camera(uint32_t pCameraWidth, uint32_t pCameraHeight, const glm::vec3& pPos,
		   float pFOV, float pNearPlane, float pFarPlane);

	glm::mat4 getViewMatrix();
	void matrix(Shader pShader, std::string_view pName);

	void moveMouse(const glm::vec2 pMousePos);
	void moveRight(float pSpeed);
	void moveLeft(float pSpeed);
	void moveForward(float pSpeed);
	void moveBackward(float pSpeed);

	void setCameraWidth(uint32_t pCameraWidth);
	void setCameraHeight(uint32_t pCameraHeight);
	void setCameraSize(uint32_t pCameraWidth, uint32_t pCameraHeight);
	void setFOV(float pFOV); 
	void setNearPlane(float pNearPlane);
	void setFarPlane(float pFarPlane);

	//
	// getters;
	// -------------------------------------
	//
	// or getEye in other words;
	//
	glm::vec3 getPosition() const noexcept;
	glm::vec3 getDirection() const noexcept;
	glm::vec3 getUpVector() const noexcept;
	glm::mat4 getCameraMatrix() const noexcept;
	uint32_t getWidthCamera() const noexcept;
	uint32_t getHeightCamera() const noexcept;
	uint32_t getFOV() const noexcept;
	uint32_t getNearPlane() const noexcept;
	uint32_t getFarPlane() const noexcept;
	// -------------------------------------

private:
	glm::vec3 mEye;
	glm::vec3 mDirection;
	glm::vec3 mUpVector;
	
	float mFOV, mNearPlane, mFarPlane;
	uint32_t mWidthCam, mHeightCam;

	glm::mat4 mCameraMatrix{ 0.0f };
};

