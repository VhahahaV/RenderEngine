//
// Created by czq on 24-6-6.
//

#include "PerspectiveCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera()
{
    updateViewMatrix();
};

void PerspectiveCamera::updateViewMatrix()
{
    glm::mat4 rotM = glm::mat4(1.0f);
    //        rotation中的x,y,z分别代表绕x,y,z轴旋转的角度，单位是度
    rotM = glm::rotate(rotM, glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotM = glm::rotate(rotM, glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotM = glm::rotate(rotM, glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 translation = mPosition + getFront() * mScale;
    glm::mat4 transM = glm::translate(glm::mat4(1.0f), translation);

    mViewMatrix = transM * rotM;
};

void PerspectiveCamera::updateView(glm::vec2 mouseDelta, const bool constrainPitch)
{
    mRotation.y += mouseDelta.x;
    mRotation.x += mouseDelta.y;
    if (constrainPitch)
    {
        if (mRotation.x > 89.0f)
            mRotation.x = 89.0f;
        if (mRotation.x < -89.0f)
            mRotation.x = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    updateViewMatrix();
}

glm::mat4 PerspectiveCamera::getViewMatrix() const
{
    return mViewMatrix;
}

glm::mat4 PerspectiveCamera::getProjMatrix(Resolution resolution) const
{
    return glm::perspective(glm::radians(mFov), resolution.w / resolution.h, mNear, mFar);
}

float PerspectiveCamera::getFov() const
{
    return mFov;
}


glm::vec3 PerspectiveCamera::getUp() const
{
    return mRotation;
}

float PerspectiveCamera::getScale() const
{
    return mScale;
}

void PerspectiveCamera::setScale(float scale)
{
    this->mScale = scale;
    auto prevLookAt = mPosition + getFront() * mScale;
    mPosition = (mPosition - prevLookAt) * (scale / mScale) + prevLookAt;
    mScale = scale;
    updateViewMatrix();
}

glm::vec3 PerspectiveCamera::getFront() const
{
    glm::vec3 front;
    front.x = -cos(glm::radians(mRotation.x)) * sin(glm::radians(mRotation.y));
    front.y = sin(glm::radians(mRotation.x));
    front.z = cos(glm::radians(mRotation.x)) * cos(glm::radians(mRotation.y));
    return glm::normalize(front);
}

glm::vec3 PerspectiveCamera::getPosition() const
{
    return mPosition;
}


void PerspectiveCamera::setFov(float fov)
{
    this->mFov = fov;
}

void PerspectiveCamera::setPosition(const glm::vec3& position)
{
    this->mPosition = position;
    updateViewMatrix();
}

void PerspectiveCamera::setRotation(glm::vec3 rotation)
{
    this->mRotation = rotation;
    updateViewMatrix();
}

void PerspectiveCamera::setTranslation(glm::vec3 translation)
{
    this->mPosition = translation;
    updateViewMatrix();
};

void PerspectiveCamera::setRotationSpeed(float rotationSpeed)
{
    this->mRotationSpeed = rotationSpeed;
}

void PerspectiveCamera::setMovementSpeed(float movementSpeed)
{
    this->mMovementSpeed = movementSpeed;
}

void PerspectiveCamera::processKeyboard(glm::vec3 delta) noexcept
{
    //        std::cout << "processKeyboard " <<delta.x << " " << delta.y << " " << delta.z << std::endl;
    //        std::cout << "mPosition " <<mPosition.x << " " << mPosition.y << " " << mPosition.z << std::endl;
    this->mPosition.x += delta.y;
    this->mPosition.z += delta.x;
    this->mPosition.y -= delta.z;
    updateViewMatrix();
}
