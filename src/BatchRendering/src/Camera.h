#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

class Camera
{
public:
	Camera();

	glm::mat4 getMatrixView();
	void moveCamera(const glm::vec2& pMousePos);
	void moveRight(float pSpeed);
	void moveLeft(float pSpeed);
	void moveForward(float pSpeed);
	void moveBackward(float pSpeed);

private:
	glm::vec3 mEye;
	glm::vec3 mDirection;
	glm::vec3 mUpVec;

	glm::mat4 mMatrixView;
};

