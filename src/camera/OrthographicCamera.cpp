//
// Created by czq on 24-6-7.
//

#include "OrthographicCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera()
{
    updateViewMatrix();
}

void OrthographicCamera::updateViewMatrix()
{
    glm::mat4 rotM = glm::mat4(1.0f);
    //        rotation中的x,y,z分别代表绕x,y,z轴旋转的角度，单位是度
    rotM = glm::rotate(rotM, glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotM = glm::rotate(rotM, glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotM = glm::rotate(rotM, glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 translation = mPosition + getFront() * mScale;
    glm::mat4 transM = glm::translate(glm::mat4(1.0f), translation);

    mViewMatrix = transM * rotM;
}


void OrthographicCamera::updateView(glm::vec2 mouseDelta, const bool constrainPitch)
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

glm::mat4 OrthographicCamera::getViewMatrix() const
{
    return mViewMatrix;
}

glm::mat4 OrthographicCamera::getProjMatrix(Resolution resolution) const
{
    // glm::mat4 rotM = glm::mat4(1.0f);
    // rotM = glm::rotate(rotM, glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    // rotM = glm::rotate(rotM, glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    // rotM = glm::rotate(rotM, glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    // glm::mat4 transM(
    //     1.0f, 0.0f, 0.0f, 0,
    //     0.0f, 1.0f, 0.0f, 0,
    //     0.0f, 0.0f, 1.0f, 0,
    //     0.f, 0.f, -4.f, 1.0f
    // );
    // return (transM) * rotM;
    // return glm::ortho(0.f, resolution.w, 0.f,resolution.h, mNear, mFar);
    // glm::mat4 res = glm::mat4(1.0f);
    return glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
}

float OrthographicCamera::getFov() const
{
    return mFov;
}


glm::vec3 OrthographicCamera::getUp() const
{
    return mRotation;
}

float OrthographicCamera::getScale() const
{
    return mScale;
}

void OrthographicCamera::setScale(float scale)
{
    this->mScale = scale;
    auto prevLookAt = mPosition + getFront() * mScale;
    mPosition = (mPosition - prevLookAt) * (scale / mScale) + prevLookAt;
    mScale = scale;
    updateViewMatrix();
}

glm::vec3 OrthographicCamera::getFront() const
{
    glm::vec3 front;
    front.x = -cos(glm::radians(mRotation.x)) * sin(glm::radians(mRotation.y));
    front.y = sin(glm::radians(mRotation.x));
    front.z = cos(glm::radians(mRotation.x)) * cos(glm::radians(mRotation.y));
    return glm::normalize(front);
}

glm::vec3 OrthographicCamera::getPosition() const
{
    return mPosition;
}


void OrthographicCamera::setFov(float fov)
{
    this->mFov = fov;
}

void OrthographicCamera::setPosition(const glm::vec3& position)
{
    this->mPosition = position;
    updateViewMatrix();
}

void OrthographicCamera::setRotation(glm::vec3 rotation)
{
    this->mRotation = rotation;
    updateViewMatrix();
}

void OrthographicCamera::setTranslation(glm::vec3 translation)
{
    this->mPosition = translation;
    updateViewMatrix();
};

void OrthographicCamera::setRotationSpeed(float rotationSpeed)
{
    this->mRotationSpeed = rotationSpeed;
}

void OrthographicCamera::setMovementSpeed(float movementSpeed)
{
    this->mMovementSpeed = movementSpeed;
}

void OrthographicCamera::processKeyboard(glm::vec3 delta) noexcept
{
    //        std::cout << "processKeyboard " <<delta.x << " " << delta.y << " " << delta.z << std::endl;
    //        std::cout << "mPosition " <<mPosition.x << " " << mPosition.y << " " << mPosition.z << std::endl;
    this->mPosition.x += delta.y;
    this->mPosition.z += delta.x;
    this->mPosition.y -= delta.z;
    updateViewMatrix();
}
