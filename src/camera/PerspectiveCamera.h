//
// Created by czq on 24-6-6.
//

#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H
#include "CameraBase.h"

class PerspectiveCamera : public CameraBase{
protected:
    // intrinsics
    float mNear = 0.1f, mFar = 1000.0f;
    float mScale = 1.0f;
    float mFov{ 70.0f };

    glm::vec3 mRotation = glm::vec3();
    glm::vec3 mPosition = glm::vec3(0.f, -1.f, -3.f);

    float mRotationSpeed = 1.0f;
    float mMovementSpeed = 1.0f;

    glm::mat4 mViewMatrix{};
    void updateViewMatrix();
public:
    PerspectiveCamera();

    void updateView(glm::vec2 mouseDelta, bool constrainPitch) override;
    void processKeyboard(glm::vec3 delta) noexcept override;
    [[nodiscard]] glm::mat4 getViewMatrix() const override;
    [[nodiscard]] glm::mat4 getProjMatrix(Resolution resolution) const override;
    [[nodiscard]] float getFov() const override;
    [[nodiscard]] glm::vec3 getUp() const override;
    [[nodiscard]] float getScale() const override;
    void setScale(float scale) override;
    [[nodiscard]] glm::vec3 getFront() const override;
    [[nodiscard]] glm::vec3 getPosition() const override;
    void setFov(float fov) override;
    void setPosition(const glm::vec3& position) override;
    void setRotation(glm::vec3 rotation);
    void setTranslation(glm::vec3 translation);
    void setRotationSpeed(float rotationSpeed);
    void setMovementSpeed(float movementSpeed);

};



#endif //PERSPECTIVECAMERA_H
