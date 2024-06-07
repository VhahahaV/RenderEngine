//
// Created by czq on 24-6-6.
//

#ifndef CAMERABASE_H
#define CAMERABASE_H


#include "glm/glm.hpp"
#include <memory>

class CameraBase;
class CameraBase{
private:
    float mNear = 0.1f, mFar = 1000.0f;
    float mScale = 1.0f;
    float mFov{ 70.0f };
public:// set and get
    virtual void processKeyboard(glm::vec3 move) noexcept = 0;
    virtual void updateView(glm::vec2 mouseDelta,const bool constrainPitch = true) = 0;
    virtual glm::mat4 getViewMatrix() const = 0;
    virtual glm::mat4 getProjMatrix(const float width, const float height) const = 0;
    virtual float getFov() const = 0;
    virtual glm::vec3 getPosition() const = 0;
    virtual glm::vec3 getUp() const = 0;
    virtual glm::vec3 getFront() const = 0;
    virtual float getScale() const = 0;

    virtual void setFov(const float fov) = 0;
    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void setScale(const float scale) = 0;
    virtual glm::mat4 getOrientation() const = 0;

};

#endif //CAMERABASE_H
