//
// Created by czq on 24-6-6.
//

#ifndef CAMERABASE_H
#define CAMERABASE_H


#include "ParseStruct.h"
#include "glm/glm.hpp"

// https://blog.csdn.net/weixin_42050609/article/details/125145422 LookAt矩阵
class CameraBase{
protected:
    float mNear = 0.1f, mFar = 1000.0f;
    float mScale = 1.0f;
    float mFov{ 70.0f };
public:// set and get
    virtual void processKeyboard(glm::vec3 move) noexcept = 0;
    virtual void updateView(glm::vec2 mouseDelta,bool constrainPitch) = 0;
    [[nodiscard]] virtual glm::mat4 getViewMatrix() const = 0;
    [[nodiscard]] virtual glm::mat4 getProjMatrix(Resolution resolution) const = 0;
    [[nodiscard]] virtual float getFov() const = 0;
    [[nodiscard]] virtual glm::vec3 getPosition() const = 0;
    [[nodiscard]] virtual glm::vec3 getUp() const = 0;
    [[nodiscard]] virtual glm::vec3 getFront() const = 0;
    [[nodiscard]] virtual float getScale() const = 0;
    virtual void setFov(float fov) = 0;
    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void setScale(float scale) = 0;
    virtual ~CameraBase() = default;

};

#endif //CAMERABASE_H
