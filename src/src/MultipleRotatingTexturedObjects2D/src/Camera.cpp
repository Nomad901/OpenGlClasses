#include "Camera.h"

Camera::Camera()
{
    mEye = glm::vec3(0.0f, 0.0f, 0.0f);
    mDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    mUpVec = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::getMatrixView()
{
    mMatrixView = glm::lookAt(mEye, mEye + mDirection, mUpVec);
    return mMatrixView;
}

void Camera::moveCamera(const glm::vec2& pMousePos)
{
    mDirection = glm::rotate(mDirection, glm::radians(pMousePos.x), mUpVec);
    glm::vec3 mTmpVec = glm::normalize(glm::cross(mDirection, mUpVec));
    mDirection = glm::rotate(mDirection, glm::radians(pMousePos.y), mTmpVec);
}

void Camera::moveRight(float pSpeed)
{
    glm::vec3 tmpVect = glm::normalize(glm::cross(mDirection, mUpVec));
    mEye += tmpVect * pSpeed;
}

void Camera::moveLeft(float pSpeed)
{
    glm::vec3 tmpVect = glm::normalize(glm::cross(mUpVec, mDirection));
    mEye += tmpVect * pSpeed;
}

void Camera::moveForward(float pSpeed)
{
    mEye += mDirection * pSpeed;
}

void Camera::moveBackward(float pSpeed)
{
    mEye -= mDirection * pSpeed;
}
